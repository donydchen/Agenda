#include "loginwindow.h"
#include "ui_loginwindow.h"
#include "mainwindow.h"
#include <QMessageBox>

using std::string;

LoginWindow::LoginWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    ui->signupwidget->hide();
}

LoginWindow::~LoginWindow()
{
    delete ui;
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
        if (agendaService_.userLogIn(username, password)) {
            //close window and open new main window with username and password
            string content = "Sign in as <font color='red'>" + username + "</font><br>"
                    + "Click OK to continue...";
            QMessageBox::information(NULL, "SignIn Success", content.c_str(),
                                     QMessageBox::Ok);
            MainWindow *mainWindow = new MainWindow(username, password);
            mainWindow->show();
            close();
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
        // sign up success
        if (agendaService_.userRegister(username, password, email, phone)) {
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
