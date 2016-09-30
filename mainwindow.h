#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "backend/src/AgendaService.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(std::string username, std::string password, QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionLogout_triggered();

    void on_actionDelete_triggered();

    void on_actionExit_triggered();

    void on_actionList_triggered();

    void on_actionList_All_Meetings_triggered();

    void on_actionList_All_Sponsor_Meetings_triggered();

    void on_actionList_All_Participant_Meetings_triggered();

    void on_actionCreate_A_Meeting_triggered();

    void on_crtMtBtn_clicked();

private:
    Ui::MainWindow *ui;
    std::string userName_;
    std::string userPassword_;
    AgendaService agendaService_;
    void printMeetings(std::list<Meeting> meetings);
};

#endif // MAINWINDOW_H
