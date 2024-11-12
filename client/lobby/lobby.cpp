#include "lobby.h"
#include "./ui_lobby.h"

Lobby::Lobby(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Lobby)
{
    ui->setupUi(this);
}

Lobby::~Lobby()
{
    delete ui;
}
