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

void Server::connectionSlot()
{
    this->tcpsocket =  this->server.nextPendingConnection();
    this->tcpsockets.push_back(this->tcpsocket);
    qDebug() << (this->tcpsockets);
    QObject::connect(this->tcpsocket, SIGNAL(readyRead()), this, SLOT(readSlot()));
    qDebug() << "Connection has arrived from remote IP " << this->tcpsocket->peerAddress().toString() << "@" << this->tcpsocket->peerPort();
    ui->client_list_table->insertRow(0);
    QTableWidgetItem * itemIP = new QTableWidgetItem(this->tcpsocket->peerAddress().toString());
    QTableWidgetItem * itemPort = new QTableWidgetItem(QString::number(this->tcpsocket->peerPort()));
    QTableWidgetItem * itemSocketDescriptor = new QTableWidgetItem(QString::number(this->tcpsocket->socketDescriptor()));
    QTableWidgetItem * itemStatus = new QTableWidgetItem("Connected");

    ui->client_list_table->setItem(0,0,itemIP);
    ui->client_list_table->setItem(0,1,itemPort);
    ui->client_list_table->setItem(0,2,itemSocketDescriptor);
    ui->client_list_table->setItem(0,3,itemStatus);
    ui->client_list_table->setCellWidget(0,4,new QPushButton("Disconnect"));

    QObject::connect((QPushButton*)ui->client_list_table->cellWidget(0,4),SIGNAL(clicked(bool)), this, SLOT(disconnectClientSlot(bool)));
    //columns width
    ui->client_list_table->setColumnWidth(0, ui->client_list_table->width()/5);
    ui->client_list_table->setColumnWidth(1, ui->client_list_table->width()/5);
    ui->client_list_table->setColumnWidth(2, ui->client_list_table->width()/5);
    ui->client_list_table->setColumnWidth(3, ui->client_list_table->width()/5);
    ui->client_list_table->setColumnWidth(4, ui->client_list_table->width()/5);
    ui->client_list_table->cellWidget(0,4)->setStyleSheet("background-color:rgb(155,0,0); color:white;");
}

void Server::readSlot()
{
    QByteArray message = this->tcpsocket->readAll();
    qDebug() << "Message from client: " << message;
    this->tcpsocket->write("Hello from server");
}

void Server::disconnectClientSlot(bool)
{
    QWidget *w = qobject_cast<QWidget *>(sender()->parent());
    if(w){
        int row = ui->client_list_table->indexAt(w->pos()).row();
        qDebug() << row;
        this->tcpsockets.at(row)->disconnectFromHost();
        this->tcpsockets.removeAt(row);
        ui->client_list_table->removeRow(row);
        ui->client_list_table->setCurrentCell(0, 0);
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
