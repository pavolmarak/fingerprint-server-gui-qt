#include "dboxserver.h"
#include "ui_dboxserver.h"

DBoxServer::DBoxServer(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DBoxServer)
{
    ui->setupUi(this);
    QObject::connect(&(this->server), SIGNAL(newConnection()), this, SLOT(connectionSlot()));
    // client list setup
    ui->client_list_table->setColumnCount(2);
    ui->client_list_table->setHorizontalHeaderLabels(QStringList() << "IP" << "Port");
}

DBoxServer::~DBoxServer()
{
    if(this->server.isListening()){
        this->server.close();
        qDebug() << "Server is stopped.";
        ui->statusBar->showMessage("Server is stopped.");
    }
    delete ui;
}

void DBoxServer::connectionSlot()
{
    this->tcpsocket =  this->server.nextPendingConnection();
    QObject::connect(this->tcpsocket, SIGNAL(readyRead()), this, SLOT(readSlot()));
    qDebug() << "Connection has arrived from remote IP " << this->tcpsocket->peerAddress().toString() << "@" << this->tcpsocket->peerPort() << " " << this->tcpsocket->peerName();
    ui->client_list_table->insertRow(0);
    QTableWidgetItem * itemIP = new QTableWidgetItem(this->tcpsocket->peerAddress().toString());
    QTableWidgetItem * itemPort = new QTableWidgetItem(QString::number(this->tcpsocket->peerPort()));
    ui->client_list_table->setItem(0,0,itemIP);
    ui->client_list_table->setItem(0,1,itemPort);
}

void DBoxServer::readSlot()
{
    QByteArray message = this->tcpsocket->readAll();
    qDebug() << "Message from client: " << message;
    this->tcpsocket->write("Hello from server");
}

void DBoxServer::on_start_server_button_clicked()
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

void DBoxServer::on_stop_server_button_clicked()
{
    if(this->server.isListening()){
        this->server.close();
        ui->start_server_button->setEnabled(true);
        qDebug() << "Server is stopped.";
        ui->statusBar->showMessage("Server is stopped.");
    }
}
