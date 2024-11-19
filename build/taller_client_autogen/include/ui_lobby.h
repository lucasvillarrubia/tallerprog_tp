/********************************************************************************
** Form generated from reading UI file 'lobby.ui'
**
** Created by: Qt User Interface Compiler version 6.2.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LOBBY_H
#define UI_LOBBY_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_lobby
{
public:
    QWidget *centralwidget;
    QPushButton *createOnePlayerMatchButton;
    QPushButton *joinMatchButton;
    QLabel *label;
    QLabel *label_2;
    QPushButton *createTwoPlayerMatchButton;
    QPushButton *createThreePlayerMatchButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *lobby)
    {
        if (lobby->objectName().isEmpty())
            lobby->setObjectName(QString::fromUtf8("lobby"));
        lobby->resize(800, 600);
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(229, 165, 10, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        QBrush brush2(QColor(255, 255, 255, 255));
        brush2.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Text, brush2);
        palette.setBrush(QPalette::Active, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        QBrush brush3(QColor(230, 97, 0, 255));
        brush3.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Window, brush3);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::BrightText, brush2);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush3);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush3);
        lobby->setPalette(palette);
        centralwidget = new QWidget(lobby);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        createOnePlayerMatchButton = new QPushButton(centralwidget);
        createOnePlayerMatchButton->setObjectName(QString::fromUtf8("createOnePlayerMatchButton"));
        createOnePlayerMatchButton->setGeometry(QRect(180, 120, 451, 71));
        QFont font;
        font.setFamilies({QString::fromUtf8("Uroob")});
        font.setPointSize(25);
        font.setBold(false);
        createOnePlayerMatchButton->setFont(font);
        createOnePlayerMatchButton->setCursor(QCursor(Qt::PointingHandCursor));
        joinMatchButton = new QPushButton(centralwidget);
        joinMatchButton->setObjectName(QString::fromUtf8("joinMatchButton"));
        joinMatchButton->setGeometry(QRect(180, 200, 451, 71));
        joinMatchButton->setFont(font);
        joinMatchButton->setCursor(QCursor(Qt::PointingHandCursor));
        joinMatchButton->setAutoFillBackground(false);
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(270, 40, 271, 71));
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Uroob")});
        font1.setPointSize(30);
        font1.setBold(true);
        label->setFont(font1);
        label->setAlignment(Qt::AlignmentFlag::AlignCenter);
        label_2 = new QLabel(centralwidget);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(270, 440, 271, 71));
        QFont font2;
        font2.setFamilies({QString::fromUtf8("Uroob")});
        font2.setPointSize(15);
        font2.setBold(false);
        label_2->setFont(font2);
        label_2->setAlignment(Qt::AlignmentFlag::AlignCenter);
        createTwoPlayerMatchButton = new QPushButton(centralwidget);
        createTwoPlayerMatchButton->setObjectName(QString::fromUtf8("createTwoPlayerMatchButton"));
        createTwoPlayerMatchButton->setGeometry(QRect(180, 280, 451, 71));
        createTwoPlayerMatchButton->setFont(font);
        createTwoPlayerMatchButton->setCursor(QCursor(Qt::PointingHandCursor));
        createThreePlayerMatchButton = new QPushButton(centralwidget);
        createThreePlayerMatchButton->setObjectName(QString::fromUtf8("createThreePlayerMatchButton"));
        createThreePlayerMatchButton->setGeometry(QRect(180, 360, 451, 71));
        createThreePlayerMatchButton->setFont(font);
        createThreePlayerMatchButton->setCursor(QCursor(Qt::PointingHandCursor));
        lobby->setCentralWidget(centralwidget);
        menubar = new QMenuBar(lobby);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 800, 22));
        lobby->setMenuBar(menubar);
        statusbar = new QStatusBar(lobby);
        statusbar->setObjectName(QString::fromUtf8("statusbar"));
        lobby->setStatusBar(statusbar);

        retranslateUi(lobby);

        QMetaObject::connectSlotsByName(lobby);
    } // setupUi

    void retranslateUi(QMainWindow *lobby)
    {
        lobby->setWindowTitle(QCoreApplication::translate("lobby", "Duck Game", nullptr));
        createOnePlayerMatchButton->setText(QCoreApplication::translate("lobby", "Create 1P Match", nullptr));
        joinMatchButton->setText(QCoreApplication::translate("lobby", "Join Match", nullptr));
        label->setText(QCoreApplication::translate("lobby", "D u c k   G a m e", nullptr));
        label_2->setText(QCoreApplication::translate("lobby", "v. 1.0.0", nullptr));
        createTwoPlayerMatchButton->setText(QCoreApplication::translate("lobby", "Create 2P Match", nullptr));
        createThreePlayerMatchButton->setText(QCoreApplication::translate("lobby", "Create 3P Match", nullptr));
    } // retranslateUi

};

namespace Ui {
    class lobby: public Ui_lobby {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOBBY_H
