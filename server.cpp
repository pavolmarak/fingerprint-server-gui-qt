#include "server.h"
#include "ui_server.h"

Server::Server(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Server)
{
    ui->setupUi(this);
    this->lastImageData = nullptr;
    this->fragmentCounter =0;
    this->bc_class = -1;
    qRegisterMetaType<PREPROCESSING_RESULTS >("PREPROCESSING_RESULTS");
    qRegisterMetaType<EXTRACTION_RESULTS >("EXTRACTION_RESULTS");
    QObject::connect(&(this->server), SIGNAL(newConnection()), this, SLOT(connectionSlot()));
    QObject::connect(&(this->p),SIGNAL(preprocessingDoneSignal(PREPROCESSING_RESULTS)), this, SLOT(preprocessingDoneSlot(PREPROCESSING_RESULTS)));
    QObject::connect(&(this->e),SIGNAL(extractionDoneSignal(EXTRACTION_RESULTS)), this, SLOT(extractionDoneSlot(EXTRACTION_RESULTS)));
    QObject::connect(&(this->m),SIGNAL(verificationDoneSignal(bool)), this, SLOT(verificationDoneSlot(bool)));

    // client list setup
    ui->client_list_table->setColumnCount(5);
    ui->client_list_table->setHorizontalHeaderLabels(QStringList() << "IP" << "Port" << "Socket ID" << "Status" << "Action");
    //columns width
    ui->client_list_table->setColumnWidth(0, ui->client_list_table->width()/6);
    ui->client_list_table->setColumnWidth(1, ui->client_list_table->width()/6);
    ui->client_list_table->setColumnWidth(2, ui->client_list_table->width()/6);
    ui->client_list_table->setColumnWidth(3, ui->client_list_table->width()/6);
    ui->client_list_table->setColumnWidth(4, ui->client_list_table->width()/6);
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
    ui->client_list_table->cellWidget(0,4)->setStyleSheet("background-color:rgb(120,120,120); color:white; font-weight:bold;");
    QObject::connect(btn,SIGNAL(clicked(bool)), this, SLOT(disconnectClientSlot(bool)));
}

void Server::readSlot()
{
    QByteArray rcvData = qobject_cast<QTcpSocket*>(sender())->readAll();
    this->img.append(rcvData);
    qDebug() << rcvData.size() << " B received.";
    //qDebug() << rcvData.data();

    if(rcvData.size() >= CONTROL_DATA_SIZE && this->fragmentCounter == 0){
        this->fragmentCounter++;
        QByteArray bca = rcvData.left(4);
        QByteArray wa = rcvData.mid(4,4);
        QByteArray ha = rcvData.mid(8,4);
        QByteArray swid = rcvData.mid(12,4);

        QDataStream d1(&bca,QIODevice::ReadWrite);
        QDataStream d2(&wa,QIODevice::ReadWrite);
        QDataStream d3(&ha,QIODevice::ReadWrite);
        QDataStream d4(&swid,QIODevice::ReadWrite);
        int bc;
        int widthImg;
        int heightImg;
        int windowID;

        d1 >> bc;
        this->bc_class = bc;
        d2 >> widthImg;
        this->img_w = widthImg;
        d3 >> heightImg;
        this->img_h = heightImg;
        d4 >> windowID;
        this->windowID = windowID;
        qDebug() << "Hlavicka prijata:\n" << "Pocet bajtov: " << bc << "\nSirka: " << widthImg << "\nVyska: " << heightImg << "\nOkno: " << windowID;
    }

    // when image is completely transferred
    if(this->img.size() == this->bc_class){
        this->fragmentCounter = 0;
        this->bc_class =0;
        qDebug() << "Whole image transferred.";
        this->img = this->img.right(this->img.size()-CONTROL_DATA_SIZE);
        cv::Mat cv_img(this->img_h, this->img_w, CV_8UC1,(unsigned char*)this->img.data());
        qDebug() << QThread::idealThreadCount() << "worker thread(s) available.";
        p.loadInput(cv_img);
        p.start();
        for(int i=0;i<ui->client_list_table->rowCount();i++){
            if((ui->client_list_table->item(i,0)->text() == qobject_cast<QTcpSocket*>(this->sender())->peerAddress().toString()) && (ui->client_list_table->item(i,1)->text().toInt() == qobject_cast<QTcpSocket*>(this->sender())->peerPort()))
            {
                ui->client_list_table->item(i,3)->setBackgroundColor(QColor("#4286f4"));
                ui->client_list_table->item(i,3)->setTextColor(QColor("#ffffff"));
                if(this->windowID == 1){
                    ui->image_from->setText(qobject_cast<QTcpSocket*>(this->sender())->peerAddress().toString() + ":" + QString::number(qobject_cast<QTcpSocket*>(this->sender())->peerPort()) + ", " + QDateTime::currentDateTime().toString("dd. MMM. yyyy") + ", " + QDateTime::currentDateTime().time().toString());
                }
                else if(this->windowID == 2){
                    ui->image_from2->setText(qobject_cast<QTcpSocket*>(this->sender())->peerAddress().toString() + ":" + QString::number(qobject_cast<QTcpSocket*>(this->sender())->peerPort()) + ", " + QDateTime::currentDateTime().toString("dd. MMM. yyyy") + ", " + QDateTime::currentDateTime().time().toString());
                }
            }
            else{
                ui->client_list_table->item(i,3)->setBackgroundColor(QColor(Qt::transparent));
            }
        }
        this->lastImageData = (unsigned char*)calloc(this->img.size(),sizeof(unsigned char));
        memcpy(this->lastImageData,(unsigned char*)this->img.data(),this->img.size());
        this->originalImage = QImage(this->lastImageData,this->img_w, this->img_h,QImage::Format_Grayscale8);
        if(this->windowID == 1){
            ui->img_box->setPixmap(QPixmap::fromImage(this->originalImage));
        }
        else if(this->windowID == 2){
            ui->img_box2->setPixmap(QPixmap::fromImage(this->originalImage));
        }

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

void Server::preprocessingDoneSlot(PREPROCESSING_RESULTS results)
{
    qDebug() << "Preprocessing done.";
    this->skeletonImage = QImage(results.imgSkeleton.data,results.imgSkeleton.cols,results.imgSkeleton.rows,QImage::Format_Grayscale8);
    //cv::imshow("Skeleton", results.imgSkeleton);
    QImage skeleton(results.imgSkeleton.data, results.imgSkeleton.cols, results.imgSkeleton.rows, QImage::Format_Grayscale8);
    this->e.loadInput(results);
    this->e.start();
}

void Server::extractionDoneSlot(EXTRACTION_RESULTS results)
{
    qDebug() << "Extraction done.";
    QImage tmp;
    if(this->windowID == 1){
        tmp = ui->img_box->grab().toImage();
    }
    else if(this->windowID == 2){
        tmp = ui->img_box2->grab().toImage();
    }

    QPainter painter(&tmp);
    QPen redpen;
    QPen bluepen;
    redpen.setWidth(1);
    bluepen.setWidth(1);
    redpen.setColor(QColor(255,0,0,255));
    bluepen.setColor(QColor(0,0,255,255));

    foreach (MINUTIA m, results.minutiaePredictedFixed) {
        if(m.type == 0){
            painter.setPen(redpen);
        }
        else{
            painter.setPen(bluepen);
        }
        painter.drawEllipse(m.xy,3,3);
    }
    if(this->windowID == 1){
        ui->img_box->setPixmap(QPixmap::fromImage(tmp));
    }
    else if(this->windowID == 2){
        ui->img_box2->setPixmap(QPixmap::fromImage(tmp));
    }

    this->fingerprints.push_back(results.minutiaePredictedFixed);
    if(this->fingerprints.size()==2){
        MATCHER matcher = suprema;
        this->m.setMatcher(matcher);
        QVector<QVector<MINUTIA>> tmp_v;
        tmp_v.push_back(this->fingerprints[1]);
        qDebug() << this->fingerprints.size() << this->fingerprints[0].size() << " vs. " << tmp_v[0].size();
        this->m.verify(this->fingerprints[0],tmp_v);
        this->fingerprints.clear();
    }


}

void Server::verificationDoneSlot(bool success)
{
    qDebug() << "VERIFICATION DONE:" << success;
    ui->verification_result->setText(QString::number(success));
    ui->verification_result->setAlignment(Qt::AlignCenter);
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

void Server::on_output_combo_activated(const QString &arg1)
{
    if(arg1 == "Original image"){
        ui->img_box->setPixmap(QPixmap::fromImage(this->originalImage));
    }
    else if(arg1 == "Skeleton"){
        ui->img_box->setPixmap(QPixmap::fromImage(this->skeletonImage));
    }
}

void Server::on_save_image_button2_clicked()
{
    ui->img_box2->grab().save(QFileDialog::getSaveFileName(nullptr,"Save image as"));
}



void Server::on_save_conn_help_clicked()
{
    QMessageBox::about(this,"Help", "<b>Help</b> goes <a href='www.google.com'>here</a>.<br>Sample list:<ul><li>First</li><li>Second</li></ul>");
}

void Server::on_conn_preset_activated(const QString &arg1)
{
    ui->server_ip->setText(arg1);
}
