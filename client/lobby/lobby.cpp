#include "lobby.h"
#include "./ui_lobby.h"

lobby::lobby(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::lobby)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::clicked, this, &lobby::on_pushButton_clicked);
}

void lobby::on_pushButton_clicked()
{
    this->close();  // This will close the window
}

// void lobby::show()
// {
//     this->show();
// }

// pushButton

lobby::~lobby()
{
    delete ui;
}
