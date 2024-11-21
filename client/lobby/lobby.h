#ifndef LOBBY_H
#define LOBBY_H

#include <QMainWindow>
#include <QCloseEvent>

QT_BEGIN_NAMESPACE
namespace Ui {
class lobby;
}
QT_END_NAMESPACE

class lobby : public QMainWindow
{
    Q_OBJECT
    bool match_button_pressed = false;
private slots:
    void on_createOnePlayerMatchButton_clicked();
    // void on_createTwoPlayerMatchButton_clicked();
    // void on_createThreePlayerMatchButton_clicked();
    // void on_joinMatchButton_clicked();
protected:
    void closeEvent(QCloseEvent *event) override;
    bool closed_by_X = false;
public:
    lobby(QWidget *parent = nullptr);
    // void show();
    bool was_closed_by_X() const { return closed_by_X; }
    ~lobby();
signals:
    void create_one_player_match();
    // void create_two_player_match();
    // void create_three_player_match();
    // void join_match();
private:
    Ui::lobby *ui;
};
#endif // LOBBY_H
