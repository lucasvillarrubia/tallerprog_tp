#ifndef LOBBY_H
#define LOBBY_H

#include <QMainWindow>
#include <QCloseEvent>
#include <map>
#include <atomic>

#include "common/gamedata.h"
#include "common/hands_on_threads/queue.h"
#include <QTimer>


QT_BEGIN_NAMESPACE
namespace Ui {
class lobby;
}
QT_END_NAMESPACE

class lobby : public QMainWindow
{
    Q_OBJECT
    bool match_button_pressed = false;
    bool start_button_pressed = false;
    // bool refresh_button_pressed = false;
    std::map<int, bool> pressed_join_buttons;
    QTimer* waiting_timer;
    void send_joining_signal(int);
private slots:
    void on_createOnePlayerMatchButton_clicked();
    void on_startMatchButton_clicked();
    // void on_createTwoPlayerMatchButton_clicked();
    // void on_createThreePlayerMatchButton_clicked();
    // void on_joinMatchButton_clicked();
    void on_refreshButton_clicked();
protected:
    void closeEvent(QCloseEvent *event) override;
    bool closed_by_X = false;
public:
    lobby(QWidget *parent = nullptr);
    // void show();
    bool was_closed_by_X() const { return closed_by_X; }
    void revert_create_button_actions();
    void revert_start_button_actions();
    void revert_join_button_actions(int);
    void reset_buttons();
    void restart_lobby();
    void set_waiting_for_match(Queue<Gamestate>&, std::atomic_bool&, int&, int&);
    void wait_for_match();
    void stop_waiting();
    void update_lobby(const std::list<Gamematch>&, int);
    ~lobby();
signals:
    void create_one_player_match();
    void start_match();
    // void create_two_player_match();
    // void create_three_player_match();
    void join_match(int);
    void refresh_lobby();
private:
    Ui::lobby *ui;
};
#endif // LOBBY_H
