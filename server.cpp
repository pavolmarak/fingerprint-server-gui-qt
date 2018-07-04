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
    QObject::connect(this->tcpsocket, SIGNAL(aboutToClose()),this, SLOT(aboutToCloseSlot()));    QObject::connect(this->tcpsocket, SIGNAL(aboutToClose()),this, SLOT(aboutToCloseSlot()));
    QObject::connect(this->tcpsocket, SIGNAL(stateChanged(QAbstractSocket::SocketState)),this, SLOT(stateChangedSlot(QAbstractSocket::SocketState)));
    qDebug() << "Connection has arrived from remote IP " << this->tcpsocket->peerAddress().toString() << "@" << this->tcpsocket->peerPort();


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
    btn->setObjectName("buttonrow_"+QString::number(this->tcpsocket->socketDescriptor()));
    qDebug() << "buttonrow_"+QString::number(this->tcpsocket->socketDescriptor());
    ui->client_list_table->setCellWidget(0,4,btn);
    ui->client_list_table->cellWidget(0,4)->setStyleSheet("background-color:rgb(175,0,0); color:white;");
    QObject::connect(btn,SIGNAL(clicked(bool)), this, SLOT(disconnectClientSlot(bool)));
}

void Server::readSlot()
{
    QByteArray message = this->tcpsocket->readAll();
    qDebug() << "Message from client: " << message;
    this->tcpsocket->write("Hello from server");
}

// slot function to handle manual client disconnection
void Server::disconnectClientSlot(bool)
{
    int num_sockets = this->tcpsockets.length();
    int socket_id_to_delete = qobject_cast<QPushButton*>(sender())->objectName().split("_").last().toInt();
    foreach (QTcpSocket* socket, this->tcpsockets) {
        if(socket->socketDescriptor() == socket_id_to_delete){
            socket->disconnectFromHost();
            if(socket == this->tcpsocket){
                this->tcpsocket = Q_NULLPTR;
            }
            if(this->tcpsockets.removeOne(socket)){
                qDebug() << "Socket ID: "<< socket_id_to_delete << " successfully deleted.";
                qDebug() << "Number of remaining sockets: " << this->tcpsockets.length();
            }
            else{
                qDebug() << "Failed to remove socket ID: " << socket_id_to_delete;
            }
            break;
        }
    }
    if(num_sockets == this->tcpsockets.length()){
        qDebug() << "You are trying to remove non-existing socket ID: " << socket_id_to_delete;
    }
    // find row with the corresponding connection
    QList<QTableWidgetItem*> items_found = ui->client_list_table->findItems(QString::number(socket_id_to_delete),Qt::MatchExactly);
    foreach (auto wi, items_found) {
        if(wi->column()==2){
            wi->setText("n/a");
            ui->client_list_table->item(ui->client_list_table->row(wi),3)->setText("Disconnected");
            ((QPushButton*)ui->client_list_table->cellWidget(ui->client_list_table->row(wi),4))->setEnabled(false);
            for(int i=0;i<4;i++){
                ui->client_list_table->item(ui->client_list_table->row(wi),i)->setBackgroundColor(QColor(255,0,0,100));
            }
            ((QPushButton*)ui->client_list_table->cellWidget(ui->client_list_table->row(wi),4))->setStyleSheet("background-color:rgb(99,99,99);color:white;");
        }
    }
}

void Server::clientDisconnectedSlot()
{
    qDebug() << "Client disconnected";
    int peerPort = qobject_cast<QTcpSocket*>(sender())->peerPort();
    for(int r=0; r < ui->client_list_table->rowCount();r++){
        if(peerPort == ui->client_list_table->item(r,1)->text().toInt()){
            ui->client_list_table->item(r,1)->setText("***");
        }
    }

}

void Server::aboutToCloseSlot()
{
    qDebug() << "About to close";
}

void Server::stateChangedSlot(QAbstractSocket::SocketState state)
{
    qDebug() << state;
    //qDebug() << "Socket ID: " << qobject_cast<QTcpSocket*>(sender())->socketDescriptor();
    if(state==QAbstractSocket::ClosingState){
        int num_sockets = this->tcpsockets.length();
        int socket_id_to_delete = qobject_cast<QTcpSocket*>(this->sender())->socketDescriptor();
        foreach (QTcpSocket* socket, this->tcpsockets) {
            if(socket->socketDescriptor() == socket_id_to_delete){
                if(socket == this->tcpsocket){
                    this->tcpsocket = Q_NULLPTR;
                }
                if(this->tcpsockets.removeOne(socket)){
                    qDebug() << "Socket ID: "<< socket_id_to_delete << " successfully deleted.";
                    qDebug() << "Number of remaining sockets: " << this->tcpsockets.length();
                }
                else{
                    qDebug() << "Failed to remove socket ID: " << socket_id_to_delete;
                }
                break;
            }
        }
        if(num_sockets == this->tcpsockets.length()){
            qDebug() << "You are trying to remove non-existing socket ID: " << socket_id_to_delete;
        }
        // find row with the corresponding connection
        QList<QTableWidgetItem*> items_found = ui->client_list_table->findItems(QString::number(socket_id_to_delete),Qt::MatchExactly);
        foreach (auto wi, items_found) {
            if(wi->column()==2){
                wi->setText("n/a");
                ui->client_list_table->item(ui->client_list_table->row(wi),3)->setText("Disconnected");
                ((QPushButton*)ui->client_list_table->cellWidget(ui->client_list_table->row(wi),4))->setEnabled(false);
                for(int i=0;i<4;i++){
                    ui->client_list_table->item(ui->client_list_table->row(wi),i)->setBackgroundColor(QColor(255,0,0,100));
                }
                ((QPushButton*)ui->client_list_table->cellWidget(ui->client_list_table->row(wi),4))->setStyleSheet("background-color:rgb(99,99,99);color:white;");
            }
        }
    }
}

void Server::on_start_server_button_clicked()
{
    this->server.listen(QHostAddress(ui->server_ip->text()),ui->server_port->text().toUInt());
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

void Server::on_stop_server_button_clicked()
{
    if(this->server.isListening()){
        this->server.close();
        ui->start_server_button->setEnabled(true);
        qDebug() << "Server is stopped.";
        ui->statusBar->showMessage("Server is stopped.");
    }
}
