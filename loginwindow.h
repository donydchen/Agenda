#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QMainWindow>
#include "mainwindow.h"
#include "backend/src/controller/json/JsonService.h"
#include "backend/src/controller/sqlite/SqliteService.h"

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit LoginWindow(std::string jsonPath, std::string sqlitePath,
                         QWidget *parent = 0);
    ~LoginWindow();

private slots:
    void on_pushButton_clicked();

    void on_signup_clicked();

    void showLoginWin();

private:
    Ui::LoginWindow *ui;
    JsonService jsonService_;
    SqliteService sqliteService_;
    AgendaService *agendaService_;
    MainWindow *mainWindow;

    void setupBackend(QString);
};

#endif // LOGINWINDOW_H
