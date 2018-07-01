#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QDebug>

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

private slots:
    void on_start_server_button_clicked();

    void on_stop_server_button_clicked();

private:
    Ui::Server *ui;
    QTcpServer server;
    QTcpSocket* tcpsocket;
};

#endif // SERVER_H
