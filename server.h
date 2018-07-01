#ifndef DBOXSERVER_H
#define DBOXSERVER_H

#include <QMainWindow>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>

namespace Ui {
class DBoxServer;
}

class DBoxServer : public QMainWindow
{
    Q_OBJECT

public:
    explicit DBoxServer(QWidget *parent = 0);
    ~DBoxServer();

public slots:
    void connectionSlot();
    void readSlot();

private slots:
    void on_start_server_button_clicked();

    void on_stop_server_button_clicked();

private:
    Ui::DBoxServer *ui;
    QTcpServer server;
    QTcpSocket* tcpsocket;
};

#endif // DBOXSERVER_H
