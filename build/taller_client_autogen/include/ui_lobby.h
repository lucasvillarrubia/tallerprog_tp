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
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    QLabel *label;
    QLabel *label_2;
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
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        pushButton->setGeometry(QRect(290, 170, 231, 81));
        QFont font;
        font.setFamilies({QString::fromUtf8("Uroob")});
        font.setPointSize(25);
        font.setBold(false);
        pushButton->setFont(font);
        pushButton->setCursor(QCursor(Qt::PointingHandCursor));
        pushButton_2 = new QPushButton(centralwidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(290, 300, 231, 81));
        pushButton_2->setFont(font);
        pushButton_2->setCursor(QCursor(Qt::PointingHandCursor));
        label = new QLabel(centralwidget);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(270, 70, 271, 71));
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
        pushButton->setText(QCoreApplication::translate("lobby", "Create Match", nullptr));
        pushButton_2->setText(QCoreApplication::translate("lobby", "Join Match", nullptr));
        label->setText(QCoreApplication::translate("lobby", "D u c k   G a m e", nullptr));
        label_2->setText(QCoreApplication::translate("lobby", "v. 1.0.0", nullptr));
    } // retranslateUi

};

namespace Ui {
    class lobby: public Ui_lobby {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LOBBY_H
