#include "lobby.h"
#include "./ui_lobby.h"
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QIcon>
#include <QAbstractButton>
#include <QString>
#include <iostream>

lobby::lobby(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::lobby)
{
    ui->setupUi(this);
    waiting_timer = nullptr;
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

void lobby::revert_join_button_actions(int match_id)
{
    pressed_join_buttons.at(match_id) = false;
}

void lobby::reset_buttons()
{
    match_button_pressed = false;
    start_button_pressed = false;
    // refresh_button_pressed = false;
    pressed_join_buttons.clear();
    ui->startMatchButton->setVisible(false);
    ui->createOnePlayerMatchButton->setVisible(true);
    ui->createOnePlayerMatchButton->setEnabled(true);
    ui->refreshButton->setEnabled(true);
    ui->scrollArea->setVisible(true);
    // clear scroll area
    QLayoutItem* item;
    while ((item = ui->verticalLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    stop_waiting();
}

void lobby::restart_lobby()
{
}

void lobby::update_lobby(const std::list<Gamematch>& matches, int player_id)
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
        if (match.player_id == player_id) {
            continue;
        }
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

void lobby::stop_waiting()
{
    if (waiting_timer) {
        waiting_timer->stop();
        waiting_timer->deleteLater();
        waiting_timer = nullptr;
    }
}

void lobby::set_waiting_for_match(Queue<Gamestate>& updates, std::atomic_bool& game_on, int& current_player_count, int& current_match)
{
    waiting_timer = new QTimer(this);
    connect(waiting_timer, &QTimer::timeout, [this, &updates, &game_on, &current_match, &current_player_count]() {
        Gamestate update;
        while (updates.try_pop(update)) {
            if (update.type == 11) {
                current_player_count = update.player_count;
                current_match = update.match_id;
                game_on.store(true);
                this->close();
                break;
            } else if (update.type == 10) {
                QMessageBox errorBox;
                errorBox.setWindowTitle("Error");
                errorBox.setText(QString::fromStdString(update.error_msg));
                errorBox.addButton(QMessageBox::Ok);
                errorBox.exec();
                current_match = 0;
                current_player_count = 0;
                reset_buttons();
                break;
            }
        }
    });
    ui->createOnePlayerMatchButton->setEnabled(false);
    ui->refreshButton->setEnabled(false);
    // ui->scrollArea->setVisible(false);
    QLayoutItem* item;
    while ((item = ui->verticalLayout->takeAt(0)) != nullptr) {
        delete item->widget();
        delete item;
    }
    // ADD TEXT WAITING
    QLabel* waitingLabel = new QLabel("Waiting for match...");
    // reduce text font size
    QFont font = waitingLabel->font();
    font.setPointSize(10);
    waitingLabel->setFont(font);
    ui->verticalLayout->addWidget(waitingLabel);
}

void lobby::wait_for_match()
{
    if (waiting_timer) {
        waiting_timer->start(1000);
    }
}

lobby::~lobby()
{
    delete ui;
}
