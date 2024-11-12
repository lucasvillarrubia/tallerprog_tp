#ifndef LOBBY_H
#define LOBBY_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class Lobby;
}
QT_END_NAMESPACE

class Lobby : public QMainWindow
{
    Q_OBJECT

public:
    Lobby(QWidget *parent = nullptr);
    ~Lobby();

private:
    Ui::Lobby *ui;
};
#endif // LOBBY_H
