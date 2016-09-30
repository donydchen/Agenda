#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(std::string username, std::string password, QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    std::string userName_;
    std::string userPassword_;
};

#endif // MAINWINDOW_H
