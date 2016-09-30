#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include "backend/src/AgendaService.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();

private slots:
    void on_pushButton_clicked();

    void on_signup_clicked();

private:
    Ui::LoginWindow *ui;
    AgendaService agendaService_;
};

#endif // LOGINWINDOW_H
