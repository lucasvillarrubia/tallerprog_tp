#ifndef LOBBY_H
#define LOBBY_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
class lobby;
}
QT_END_NAMESPACE

class lobby : public QMainWindow
{
    Q_OBJECT
private slots:
    void on_createOnePlayerMatchButton_clicked();
    void on_createTwoPlayerMatchButton_clicked();
    void on_createThreePlayerMatchButton_clicked();
    void on_joinMatchButton_clicked();
public:
    lobby(QWidget *parent = nullptr);
    // void show();
    ~lobby();
signals:
    void create_one_player_match();
    void create_two_player_match();
    void create_three_player_match();
    void join_match();
private:
    Ui::lobby *ui;
};
#endif // LOBBY_H
