#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QFileDialog>
#include <QDateTime>

// DBOX modules
#include "preprocessing.h"
#include "extraction.h"
#include "matcher.h"

// OpenCV
#include "opencv2/world.hpp"

#define IMG_SIZE 153600
#define IMG_WIDTH 320
#define IMG_HEIGHT 480

namespace Ui {
class Server;
}

class Server : public QMainWindow
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    ~Server();

public slots:
    void connectionSlot();
    void readSlot();
    void disconnectClientSlot(bool);
    void clientDisconnectedSlot();
    void stateChangedSlot(QAbstractSocket::SocketState);
    void preprocessingDoneSlot(PREPROCESSING_RESULTS results);
    void extractionDoneSlot(EXTRACTION_RESULTS results);
private slots:
    void on_start_server_button_clicked();
    void on_stop_server_button_clicked();

    void on_save_image_button_clicked();

    void on_output_combo_activated(const QString &arg1);

private:
    Ui::Server *ui;
    QTcpServer server;
    QTcpSocket* tcpsocket;
    QList<QTcpSocket*> tcpsockets;
    QByteArray img;
    Preprocessing p;
    Extraction e;
    QImage originalImage, skeletonImage;
    unsigned char* lastImageData;
};

#endif // SERVER_H
