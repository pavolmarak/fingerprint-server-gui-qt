#include "dboxserver.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DBoxServer w;
    w.show();

    return a.exec();
}
