#include "lobby.h"
#include "./ui_lobby.h"
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>

lobby::lobby(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::lobby)
{
    ui->setupUi(this);
    connect(ui->createOnePlayerMatchButton, &QPushButton::clicked, this, &lobby::on_createOnePlayerMatchButton_clicked);
    connect(ui->startMatchButton, &QPushButton::clicked, this, &lobby::on_startMatchButton_clicked);
    // connect(ui->joinMatchButton, &QPushButton::clicked, this, &lobby::on_joinMatchButton_clicked);
    connect(ui->refreshButton, &QPushButton::clicked, this, &lobby::on_refreshButton_clicked);
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
        ui->createOnePlayerMatchButton->setVisible(false);
        ui->startMatchButton->setVisible(true);
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

void lobby::on_startMatchButton_clicked()
{
    if (not start_button_pressed) {
        start_button_pressed = true;
        emit start_match();
        this->close();
    }
}

void lobby::on_refreshButton_clicked()
{
    // if (not refresh_button_pressed) {
        // refresh_button_pressed = true;
        emit refresh_lobby();
    // }
}

void lobby::closeEvent(QCloseEvent *event) {
    if (!start_button_pressed) {
        closed_by_X = true;
    }
    event->accept();
}

void lobby::revert_create_button_actions()
{
    match_button_pressed = false;
    ui->startMatchButton->setVisible(false);
    ui->createOnePlayerMatchButton->setVisible(true);
}

void lobby::revert_start_button_actions()
{
    start_button_pressed = false;
}

void lobby::reset_buttons()
{
    match_button_pressed = false;
    start_button_pressed = false;
    // refresh_button_pressed = false;
    pressed_join_buttons.clear();
    ui->startMatchButton->setVisible(false);
    ui->createOnePlayerMatchButton->setVisible(true);
}

void lobby::update_lobby(const std::list<Gamematch>& matches)
{
    // Clear the existing layout
    QLayoutItem* item;
    while ((item = ui->verticalLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    // Clear map of pressed buttons
    pressed_join_buttons.clear();

    // Add new match blocks
    for (const auto& match : matches) {
        QWidget* matchWidget = new QWidget();
        matchWidget->setFixedHeight(50);
        QHBoxLayout* matchLayout = new QHBoxLayout(matchWidget);

        QLabel* matchInfo = new QLabel(QString("Match ID: %1, Creator ID: %2, Players: %3")
                                       .arg(match.match_id)
                                       .arg(match.player_id)
                                       .arg(match.players_count));
        QPushButton* joinButton = new QPushButton("JOIN");

        connect(joinButton, &QPushButton::clicked, [this, match]() {
            this->send_joining_signal(match.match_id);
        });

        matchLayout->addWidget(matchInfo);
        matchLayout->addWidget(joinButton);
        ui->verticalLayout->addWidget(matchWidget);
        pressed_join_buttons.insert({match.match_id, false});
    }
}

void lobby::send_joining_signal(int match_id)
{
    if (not pressed_join_buttons.at(match_id)) {
        pressed_join_buttons.at(match_id) = true;
        emit join_match(match_id);
    }
}

lobby::~lobby()
{
    delete ui;
}
