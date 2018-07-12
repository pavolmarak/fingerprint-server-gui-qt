#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>
#include <QFileDialog>
#include <QDateTime>
#include <QTableWidget>
#include <QMessageBox>
#include <QLabel>

// DBOX modules
#include "preprocessing.h"
#include "extraction.h"
#include "matcher.h"

// OpenCV
#include "opencv2/world.hpp"

// Suprema BioMini Slim image size in bytes
#define SUPREMA_IMG_SIZE 153600

// Suprema BioMini Slim image size in pixels
#define SUPREMA_IMG_WIDTH 320
#define SUPREMA_IMG_HEIGHT 480

// control data size in bytes
#define CONTROL_DATA_SIZE 16


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

    void on_save_conn_help_clicked();

    void on_conn_preset_activated(const QString &arg1);

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
    int windowID;
};

#endif // SERVER_H
