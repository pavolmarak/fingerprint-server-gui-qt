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

#define SUPREMA_IMG_SIZE 153600
#define IMG_WIDTH 320
#define IMG_HEIGHT 480
#define HEADER_SIZE 12

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
    void verificationDoneSlot(bool success);
private slots:
    void on_start_server_button_clicked();
    void on_stop_server_button_clicked();

    void on_save_image_button_clicked();

    void on_output_combo_activated(const QString &arg1);

    void on_save_image_button2_clicked();

private:
    Ui::Server *ui;
    QTcpServer server;
    QTcpSocket* tcpsocket;
    QList<QTcpSocket*> tcpsockets;
    QByteArray img;
    Preprocessing p;
    Extraction e;
    Matcher m;
    QImage originalImage, skeletonImage;
    unsigned char* lastImageData;
    QVector<QVector<MINUTIA>> fingerprints;
    int fragmentCounter;
    int bc_class;
    int img_w, img_h;
};

#endif // SERVER_H
