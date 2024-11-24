#include "lobby.h"
#include "./ui_lobby.h"

lobby::lobby(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::lobby)
{
    ui->setupUi(this);
    connect(ui->createOnePlayerMatchButton, &QPushButton::clicked, this, &lobby::on_createOnePlayerMatchButton_clicked);
    // connect(ui->joinMatchButton, &QPushButton::clicked, this, &lobby::on_joinMatchButton_clicked);
    // connect(ui->createTwoPlayerMatchButton, &QPushButton::clicked, this, &lobby::on_createTwoPlayerMatchButton_clicked);
    // connect(ui->createThreePlayerMatchButton, &QPushButton::clicked, this, &lobby::on_createThreePlayerMatchButton_clicked);
}

void lobby::on_createOnePlayerMatchButton_clicked()
{
    // This will emit the signal
    // qDebug() << "Button clicked!";
    // // Prevent multiple clicks
    // if (match_button_pressed) {
    //     return;
    // }
    // match_button_pressed = true;
    // ui->createOnePlayerMatchButton->setEnabled(false);
    // this->blockSignals(true);
    // // Emit signal before any cleanup
    // emit create_one_player_match();
    
    // // Schedule deletion for next event loop
    // this->close();
    if (not match_button_pressed) {
        match_button_pressed = true;
        emit create_one_player_match();
        this->close();
    }
}

// void lobby::on_createTwoPlayerMatchButton_clicked()
// {
//     // This will emit the signal
//     emit create_two_player_match();
// }

// void lobby::on_createThreePlayerMatchButton_clicked()
// {
//     // This will emit the signal
//     emit create_three_player_match();
// }

// void lobby::on_joinMatchButton_clicked()
// {
//     // This will emit the signal
//     emit join_match();
// }

void lobby::closeEvent(QCloseEvent *event) {
    if (!match_button_pressed) {
        closed_by_X = true;
    }
    event->accept();
}

lobby::~lobby()
{
    delete ui;
}
