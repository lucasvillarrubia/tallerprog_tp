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
    void on_pushButton_clicked();
public:
    lobby(QWidget *parent = nullptr);
    // void show();
    ~lobby();

private:
    Ui::lobby *ui;
};
#endif // LOBBY_H
