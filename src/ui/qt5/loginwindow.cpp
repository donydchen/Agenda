#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QMessageBox>
#include <QIcon>
#include <QDir>

using std::string;

LoginWindow::LoginWindow(string jsonPath, string sqlitePath, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow),
    jsonService_(jsonPath),
    sqliteService_(sqlitePath)
{
    ui->setupUi(this);
    ui->signupwidget->hide();

    agendaService_ = &sqliteService_;
    mainWindow = NULL;

    setFixedSize(size());
    QIcon::setThemeName("ubuntu-mono-dark");
}

LoginWindow::~LoginWindow()
{
    delete ui;
    delete mainWindow;
}

void LoginWindow::on_pushButton_clicked()
{
    if (ui->loginwidget->isHidden()) {
        ui->signupwidget->hide();
        ui->loginwidget->show();
    }
    else {
        QString qUsername = ui->username->text();
        QString qPassword = ui->password->text();
        string username = qUsername.toUtf8().constData();
        string password = qPassword.toUtf8().constData();
        setupBackend(ui->backendBox->currentText());
        if (agendaService_->userLogIn(username, password)) {
            //close window and open new main window with username and password
            string content = "Sign in as <font color='red'>" + username + "</font><br>"
                    + "Click OK to continue...";
            //string content = QDir::currentPath().toUtf8().constData();
            //string content = QCoreApplication::applicationDirPath().toUtf8().constData();
            QMessageBox::information(NULL, "SignIn Success", content.c_str(),
                                     QMessageBox::Ok);

            if (mainWindow == NULL) {
                mainWindow = new MainWindow();
                connect(mainWindow, SIGNAL(mainWinClose()), this, SLOT(showLoginWin()));
            }
            mainWindow->updateWin(username, password, agendaService_);
            mainWindow->show();
            hide();
        } else {
            //pop out error message message box
            QMessageBox::warning(NULL, "Login Failed", "Please check out your Username or Password!");
        }
    }
}

void LoginWindow::on_signup_clicked()
{
    if (ui->signupwidget->isHidden()) {
        ui->loginwidget->hide();
        ui->signupwidget->show();
    }
    // do the sign up job
    else {
        string username = ui->upUsername->text().toUtf8().constData();
        string password = ui->upPass->text().toUtf8().constData();
        string email = ui->email->text().toUtf8().constData();
        string phone = ui->phone->text().toUtf8().constData();
        setupBackend(ui->backendBox->currentText());
        // sign up success
        if (agendaService_->userRegister(username, password, email, phone)) {
            string content = "Your username is <font color='red'>" + username + "</font>";
            QMessageBox::information(NULL, "SignUp Success", content.c_str(),
                                     QMessageBox::Ok);
            ui->signupwidget->hide();
            ui->loginwidget->show();
            ui->username->setText(QString::fromStdString(username));
            //ui->password->setText(QString::fromStdString(password));
        }
        else {
            QMessageBox::warning(NULL, "SignUp Failed",
                                 "Please check out the information your have typed in!");
        }
    }

}


void LoginWindow::showLoginWin() {
    /*if (mainWindow != NULL) {
        delete mainWindow;
        mainWindow = NULL;
    }*/
    show();
}


void LoginWindow::setupBackend(QString backend) {
    if (backend == QString::fromStdString("JSON")) {
        agendaService_ = &jsonService_;
    } else if (backend == QString::fromStdString("SQLite")) {
        agendaService_ = &sqliteService_;
    } else {
        // other service
    }
}
