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
    QObject::connect(this->tcpsocket, SIGNAL(disconnected()),this, SLOT(clientDisconnectedSlot()));
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
    QPushButton* btn = new QPushButton("Disconnect");
    btn->setObjectName("row_"+QString::number(ui->client_list_table->rowCount()-1));
    qDebug() << "row_"+QString::number(ui->client_list_table->rowCount()-1);
    ui->client_list_table->setCellWidget(0,4,btn);


    QObject::connect(btn,SIGNAL(clicked(bool)), this, SLOT(disconnectClientSlot(bool)));
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
    QString btn_name = qobject_cast<QWidget*>(this->sender())->objectName();
    QStringList qsl = btn_name.split("_");
    int row_id = qsl.last().toInt();
    this->tcpsockets.at(row_id)->disconnectFromHost();
    //this->tcpsockets.removeAt(row_id);
    qDebug() << "Row count: " << ui->client_list_table->rowCount();
    qDebug() << "Row to delete: " << (ui->client_list_table->rowCount()-1)-row_id;
    ((QPushButton*)(ui->client_list_table->cellWidget((ui->client_list_table->rowCount()-1)-row_id, 4)))->setStyleSheet("background-color:gray;color:white;");
    (ui->client_list_table->cellWidget((ui->client_list_table->rowCount()-1)-row_id, 4))->setEnabled(false);
    ui->client_list_table->item((ui->client_list_table->rowCount()-1)-row_id,3)->setText("Disconnected");
    //ui->client_list_table->removeRow((ui->client_list_table->rowCount()-1)-row_id);
}

void Server::clientDisconnectedSlot()
{
    qDebug() << qobject_cast<QTcpSocket*>(this->sender())->socketDescriptor();

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
