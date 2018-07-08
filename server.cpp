#include "server.h"
#include "ui_server.h"

Server::Server(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);
    QObject::connect(&(this->server), SIGNAL(newConnection()), this, SLOT(connectionSlot()));
    // client list setup
    ui->client_list_table->setColumnCount(5);
    ui->client_list_table->setHorizontalHeaderLabels(QStringList() << "IP" << "Port" << "Socket ID" << "Status" << "Action");
    //columns width
    ui->client_list_table->setColumnWidth(0, ui->client_list_table->width()/5);
    ui->client_list_table->setColumnWidth(1, ui->client_list_table->width()/5);
    ui->client_list_table->setColumnWidth(2, ui->client_list_table->width()/5);
    ui->client_list_table->setColumnWidth(3, ui->client_list_table->width()/5);
    ui->client_list_table->setColumnWidth(4, ui->client_list_table->width()/5);
}

Server::~Server()
{
    qDebug() << "DESTRUCTOR: Number of active TCP sockets is " << this->tcpsockets.length();
    if(this->server.isListening()){
        this->server.close();
        qDebug() << "Server is stopped.";
        ui->statusBar->showMessage("Server is stopped.");
    }
    delete ui;
}

// slot for a new connection
void Server::connectionSlot()
{
    // storing the new connection as current connection
    this->tcpsocket =  this->server.nextPendingConnection();

    // pushing the new connection to the QList of connections
    this->tcpsockets.push_back(this->tcpsocket);

    // connecting socket signal to their handling slots
    QObject::connect(this->tcpsocket, SIGNAL(readyRead()), this, SLOT(readSlot()));
    QObject::connect(this->tcpsocket, SIGNAL(disconnected()),this, SLOT(clientDisconnectedSlot()));
    QObject::connect(this->tcpsocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),this, SLOT(stateChangedSlot(QAbstractSocket::SocketState)));

    qDebug() << "[NEW CLIENT] Connection has arrived from remote IP "
             << this->tcpsocket->peerAddress().toString()
             << "@"
             << this->tcpsocket->peerPort()
             << "buttonrow_"+ this->tcpsocket->peerAddress().toString() + "_" + QString::number(this->tcpsocket->peerPort()) + "_" + QString::number(this->tcpsocket->socketDescriptor());


    // creating a new table row for the new connection
    ui->client_list_table->insertRow(0);
    QTableWidgetItem * itemIP = new QTableWidgetItem(this->tcpsocket->peerAddress().toString());
    QTableWidgetItem * itemPort = new QTableWidgetItem(QString::number(this->tcpsocket->peerPort()));
    QTableWidgetItem * itemSocketDescriptor = new QTableWidgetItem(QString::number(this->tcpsocket->socketDescriptor()));
    QTableWidgetItem * itemStatus = new QTableWidgetItem("Connected");

    ui->client_list_table->setItem(0,0,itemIP);
    ui->client_list_table->setItem(0,1,itemPort);
    ui->client_list_table->setItem(0,2,itemSocketDescriptor);
    ui->client_list_table->setItem(0,3,itemStatus);

    // creating Disconnect button for each connection
    QPushButton* btn = new QPushButton("Disconnect");
    // button name is formed as button + ip + port + socket_id
    btn->setObjectName("buttonrow_"+ this->tcpsocket->peerAddress().toString() + "_" + QString::number(this->tcpsocket->peerPort()) + "_" + QString::number(this->tcpsocket->socketDescriptor()));
    ui->client_list_table->setCellWidget(0,4,btn);
    ui->client_list_table->cellWidget(0,4)->setStyleSheet("background-color:rgb(175,0,0); color:white;");
    QObject::connect(btn,SIGNAL(clicked(bool)), this, SLOT(disconnectClientSlot(bool)));
}

void Server::readSlot()
{
    QByteArray rcvData = qobject_cast<QTcpSocket*>(sender())->readAll();
    this->img.append(rcvData);
    qDebug() << rcvData.size() << " B reveived.";
    if(this->img.size() == IMG_SIZE){
        qDebug() << "Whole image transferred.";
        for(int i=0;i<ui->client_list_table->rowCount();i++){
            if((ui->client_list_table->item(i,0)->text() == qobject_cast<QTcpSocket*>(this->sender())->peerAddress().toString()) && (ui->client_list_table->item(i,1)->text().toInt() == qobject_cast<QTcpSocket*>(this->sender())->peerPort()))
            {
                ui->client_list_table->item(i,3)->setBackgroundColor(QColor(Qt::green));
            }
            else{
                ui->client_list_table->item(i,3)->setBackgroundColor(QColor(Qt::transparent));
            }
        }
        QImage fing_img((unsigned char*)this->img.data(),IMG_WIDTH,IMG_HEIGHT,QImage::Format_Grayscale8);
        ui->img_box->setPixmap(QPixmap::fromImage(fing_img));
        this->img.clear();
    }
}

// slot function to handle manual client disconnection initiated by server
void Server::disconnectClientSlot(bool)
{
    int num_sockets = this->tcpsockets.length();
    QString ip_to_delete = qobject_cast<QPushButton*>(sender())->objectName().split("_").at(1);
    int port_to_delete = qobject_cast<QPushButton*>(sender())->objectName().split("_").at(2).toInt();
    int socket_id_to_delete = qobject_cast<QPushButton*>(sender())->objectName().split("_").at(3).toInt();
    foreach (QTcpSocket* socket, this->tcpsockets) {
        if((socket->socketDescriptor() == socket_id_to_delete) && (socket->peerPort() == port_to_delete) && (socket->peerAddress().toString() == ip_to_delete)){
            socket->disconnectFromHost();
            if(socket == this->tcpsocket){
                this->tcpsocket = Q_NULLPTR;
                if(!this->tcpsockets.isEmpty()){
                    this->tcpsocket = this->tcpsockets.last();
                }
            }
            if(this->tcpsockets.removeOne(socket)){
                qDebug() << "Socket ID: "<< socket_id_to_delete << " from IP: " << ip_to_delete << ":" << port_to_delete << " successfully deleted.";
                qDebug() << "Number of remaining sockets: " << this->tcpsockets.length();
            }
            else{
                qDebug() << "Failed to remove socket ID: " << socket_id_to_delete << " from IP: " << ip_to_delete << ":" << port_to_delete;
            }
            break;
        }
    }
    if(num_sockets == this->tcpsockets.length()){
        qDebug() << "You are trying to remove non-existing socket with ID: " << socket_id_to_delete << " and port: " << port_to_delete;
    }
    // find row with the corresponding connection
    QList<QTableWidgetItem*> items_found = ui->client_list_table->findItems(QString::number(port_to_delete),Qt::MatchExactly);
    // for all items in the table that contains the given port name
    foreach (auto wi, items_found) {
        // if we found port name in column no. 1 and the corresponding row also contains the given IP and socket descriptor
        if((wi->column()==1) && (ui->client_list_table->item(wi->row(),2)->text().toInt()==socket_id_to_delete) && (ui->client_list_table->item(wi->row(),0)->text()==ip_to_delete)){
            ui->client_list_table->removeRow(wi->row());
        }
    }
}


// function to handle situations when sockets are closed from outside
void Server::clientDisconnectedSlot()
{
    qDebug() << "Client disconnected";

    int num_sockets = this->tcpsockets.length();
    QString ip_to_delete = qobject_cast<QTcpSocket*>(sender())->peerAddress().toString();
    int port_to_delete = qobject_cast<QTcpSocket*>(sender())->peerPort();
    foreach (QTcpSocket* socket, this->tcpsockets) {
        if((socket->peerPort() == port_to_delete) && (socket->peerAddress().toString() == ip_to_delete)){
            if(socket == this->tcpsocket){
                this->tcpsocket = Q_NULLPTR;
                if(!this->tcpsockets.isEmpty()){
                    this->tcpsocket = this->tcpsockets.last();
                }
            }
            if(this->tcpsockets.removeOne(socket)){
                qDebug() << "Socket from IP: " << ip_to_delete << ":" << port_to_delete << " successfully deleted.";
                qDebug() << "Number of remaining sockets: " << this->tcpsockets.length();
            }
            else{
                qDebug() << "Failed to remove socket from IP: " << ip_to_delete << ":" << port_to_delete;
            }
            break;
        }
    }
    if(num_sockets == this->tcpsockets.length()){
        qDebug() << "You are trying to remove non-existing socket from: " << ip_to_delete << ":" << port_to_delete;
    }
    // find row with the corresponding connection
    QList<QTableWidgetItem*> items_found = ui->client_list_table->findItems(QString::number(port_to_delete),Qt::MatchExactly);
    // for all items in the table that contains the given port name
    foreach (auto wi, items_found) {
        // if we found port name in column no. 1 and the corresponding row also contains the given IP
        if((wi->column()==1) && (ui->client_list_table->item(wi->row(),0)->text()==ip_to_delete)){
            ui->client_list_table->removeRow(wi->row());
        }
    }
}

// function to handle different socket states
void Server::stateChangedSlot(QAbstractSocket::SocketState state)
{

}

// function to the start server and make it listen on predefined port for new connections
void Server::on_start_server_button_clicked()
{
    bool ret = this->server.listen(QHostAddress(ui->server_ip->text()),ui->server_port->text().toUInt());
    ret ? qDebug() << "Server was started sucessfully." : qDebug() << "Server failed to start.";
    if(this->server.isListening()){
        qDebug() << "Server is listening ...";
        ui->statusBar->showMessage("Server is listening ...");
        ui->start_server_button->setEnabled(false);
    }
    else{
        qDebug() << "Server is NOT listening ...";
        ui->statusBar->showMessage("Server is NOT listening ...");
    }
}

// function to stop the server but all established connections remain intact
void Server::on_stop_server_button_clicked()
{
    if(this->server.isListening()){
        this->server.close();
        ui->start_server_button->setEnabled(true);
        qDebug() << "Server is stopped.";
        ui->statusBar->showMessage("Server is stopped.");
    }
}

void Server::on_save_image_button_clicked()
{
    ui->img_box->grab().save(QFileDialog::getSaveFileName(nullptr,"Save image as"));
}
