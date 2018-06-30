/********************************************************************************
** Form generated from reading UI file 'dboxserver.ui'
**
** Created by: Qt User Interface Compiler version 5.10.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DBOXSERVER_H
#define UI_DBOXSERVER_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_DBoxServer
{
public:
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_4;
    QHBoxLayout *horizontalLayout_3;
    QGroupBox *server_config;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_5;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *ip_label;
    QLineEdit *server_ip;
    QHBoxLayout *horizontalLayout_2;
    QLabel *port_label;
    QLineEdit *server_port;
    QSpacerItem *horizontalSpacer;
    QSpacerItem *verticalSpacer;
    QHBoxLayout *horizontalLayout_6;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *start_server_button;
    QPushButton *stop_server_button;
    QGroupBox *client_list;
    QVBoxLayout *verticalLayout_3;
    QTableWidget *client_list_table;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *DBoxServer)
    {
        if (DBoxServer->objectName().isEmpty())
            DBoxServer->setObjectName(QStringLiteral("DBoxServer"));
        DBoxServer->resize(543, 494);
        centralWidget = new QWidget(DBoxServer);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_4 = new QHBoxLayout(centralWidget);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        server_config = new QGroupBox(centralWidget);
        server_config->setObjectName(QStringLiteral("server_config"));
        verticalLayout_2 = new QVBoxLayout(server_config);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        ip_label = new QLabel(server_config);
        ip_label->setObjectName(QStringLiteral("ip_label"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ip_label->sizePolicy().hasHeightForWidth());
        ip_label->setSizePolicy(sizePolicy);
        ip_label->setMinimumSize(QSize(100, 0));
        ip_label->setMaximumSize(QSize(100, 16777215));

        horizontalLayout->addWidget(ip_label);

        server_ip = new QLineEdit(server_config);
        server_ip->setObjectName(QStringLiteral("server_ip"));

        horizontalLayout->addWidget(server_ip);


        verticalLayout->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        port_label = new QLabel(server_config);
        port_label->setObjectName(QStringLiteral("port_label"));
        sizePolicy.setHeightForWidth(port_label->sizePolicy().hasHeightForWidth());
        port_label->setSizePolicy(sizePolicy);
        port_label->setMinimumSize(QSize(100, 0));
        port_label->setMaximumSize(QSize(100, 16777215));

        horizontalLayout_2->addWidget(port_label);

        server_port = new QLineEdit(server_config);
        server_port->setObjectName(QStringLiteral("server_port"));

        horizontalLayout_2->addWidget(server_port);


        verticalLayout->addLayout(horizontalLayout_2);


        horizontalLayout_5->addLayout(verticalLayout);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);


        verticalLayout_2->addLayout(horizontalLayout_5);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_2);

        start_server_button = new QPushButton(server_config);
        start_server_button->setObjectName(QStringLiteral("start_server_button"));
        QSizePolicy sizePolicy1(QSizePolicy::Preferred, QSizePolicy::Preferred);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(start_server_button->sizePolicy().hasHeightForWidth());
        start_server_button->setSizePolicy(sizePolicy1);

        horizontalLayout_6->addWidget(start_server_button);

        stop_server_button = new QPushButton(server_config);
        stop_server_button->setObjectName(QStringLiteral("stop_server_button"));

        horizontalLayout_6->addWidget(stop_server_button);


        verticalLayout_2->addLayout(horizontalLayout_6);


        horizontalLayout_3->addWidget(server_config);

        client_list = new QGroupBox(centralWidget);
        client_list->setObjectName(QStringLiteral("client_list"));
        QSizePolicy sizePolicy2(QSizePolicy::Expanding, QSizePolicy::Preferred);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(client_list->sizePolicy().hasHeightForWidth());
        client_list->setSizePolicy(sizePolicy2);
        verticalLayout_3 = new QVBoxLayout(client_list);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        client_list_table = new QTableWidget(client_list);
        client_list_table->setObjectName(QStringLiteral("client_list_table"));

        verticalLayout_3->addWidget(client_list_table);


        horizontalLayout_3->addWidget(client_list);


        horizontalLayout_4->addLayout(horizontalLayout_3);

        DBoxServer->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(DBoxServer);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 543, 20));
        DBoxServer->setMenuBar(menuBar);
        mainToolBar = new QToolBar(DBoxServer);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        DBoxServer->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(DBoxServer);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        DBoxServer->setStatusBar(statusBar);

        retranslateUi(DBoxServer);

        QMetaObject::connectSlotsByName(DBoxServer);
    } // setupUi

    void retranslateUi(QMainWindow *DBoxServer)
    {
        DBoxServer->setWindowTitle(QApplication::translate("DBoxServer", "DBoxServer", nullptr));
        server_config->setTitle(QApplication::translate("DBoxServer", "Server configuration", nullptr));
        ip_label->setText(QApplication::translate("DBoxServer", "IP:", nullptr));
        server_ip->setText(QApplication::translate("DBoxServer", "147.175.106.8", nullptr));
        server_ip->setPlaceholderText(QApplication::translate("DBoxServer", "147.175.106.8", nullptr));
        port_label->setText(QApplication::translate("DBoxServer", "Port:", nullptr));
        server_port->setText(QApplication::translate("DBoxServer", "55555", nullptr));
        server_port->setPlaceholderText(QApplication::translate("DBoxServer", "55555", nullptr));
        start_server_button->setText(QApplication::translate("DBoxServer", "Start", nullptr));
        stop_server_button->setText(QApplication::translate("DBoxServer", "Stop", nullptr));
        client_list->setTitle(QApplication::translate("DBoxServer", "Client list", nullptr));
    } // retranslateUi

};

namespace Ui {
    class DBoxServer: public Ui_DBoxServer {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DBOXSERVER_H
