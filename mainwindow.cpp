#include "mainwindow.h"
#include "ui_mainwindow.h"
using std::string;

MainWindow::MainWindow(string username, string password, QWidget *parent) :
    userName_(username), userPassword_(password), QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
