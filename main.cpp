#include "server.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Server server;
    server.setWindowIcon(QIcon("server.png"));
    server.show();

    // server status setup
    server.setServerStatus(false);

    return a.exec();
}
