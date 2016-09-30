#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "loginwindow.h"
#include <QMessageBox>
#include <QStandardItemModel>

using std::string;
using std::list;

MainWindow::MainWindow(string username, string password, QWidget *parent) :
    userName_(username), userPassword_(password), QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->actionLogout->setText(ui->actionLogout->text() + " \"" + userName_.c_str() + "\"");
    ui->actionDelete->setText(ui->actionDelete->text() + " \"" + userPassword_.c_str() + "\"");
    setWindowTitle(windowTitle() + " : " + userName_.c_str());

    ui->tableView->hide();
    ui->createmtwidget->hide();
}

MainWindow::~MainWindow()
{
    agendaService_.quitAgenda();
    delete ui;
}


void MainWindow::on_actionLogout_triggered()
{
    string content = "Do you really want to logout " + userName_ + "?";
    int ans = QMessageBox::question(NULL, "Logout User",
                          content.c_str(),
                          QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (ans == QMessageBox::Yes) {
        LoginWindow *loginWindow = new LoginWindow();
        loginWindow->show();
        close();
    }
}


void MainWindow::on_actionDelete_triggered()
{
    string content = "Do you really want to delete " + userName_ + "?";
    int ans = QMessageBox::question(NULL, "Delete User",
                                    content.c_str(),
                                    QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (ans == QMessageBox::Yes) {
        if (agendaService_.deleteUser(userName_, userPassword_)) {
            content = "Successfully delete user " + userName_ +
                    "<br>Click OK to continue...";
            QMessageBox::information(NULL, "Delete Succeed",
                                     content.c_str(),
                                     QMessageBox::Ok);
            LoginWindow *loginWindow = new LoginWindow();
            loginWindow->show();
            close();
        }
        else {
            content = "Fail to delete user " + userName_;
            QMessageBox::warning(NULL, "Delete Failed", content.c_str());
        }
    }
}


void MainWindow::on_actionExit_triggered()
{
    agendaService_.quitAgenda();
    qApp->closeAllWindows();
}

/**
 * @brief MainWindow::on_actionList_triggered
 * to list all user in Agenda
 */
void MainWindow::on_actionList_triggered()
{
    //ui->tableView->hide();
    list<User> users = agendaService_.listAllUsers();
    if (!users.empty()) {
        int rows = users.size();
        QStandardItemModel *model = new QStandardItemModel(rows, 3, this);
        model->setHorizontalHeaderItem(0, new QStandardItem(QString("Name")));
        model->setHorizontalHeaderItem(1, new QStandardItem(QString("Email")));
        model->setHorizontalHeaderItem(2, new QStandardItem(QString("Phone")));
        list<User>::iterator it;
        int i;
        for (i = 0, it = users.begin(); it != users.end(); ++i, ++it) {
            QStandardItem *nameItem = new QStandardItem(QString(it->getName().c_str()));
            QStandardItem *emailItem = new QStandardItem(QString(it->getEmail().c_str()));
            QStandardItem *phoneItem = new QStandardItem(QString(it->getPhone().c_str()));
            model->setItem(i, 0, nameItem);
            model->setItem(i, 1, emailItem);
            model->setItem(i, 2, phoneItem);
        }
        ui->tableView->setModel(model);
        ui->tableView->show();
    }
    else {
        QMessageBox::information(NULL, "List User", "Sorry, no user exist!", QMessageBox::Ok);
        ui->tableView->hide();
    }
}


/**
 * @brief MainWindow::on_actionList_All_Meetings_triggered
 * list all meetings
 */
void MainWindow::on_actionList_All_Meetings_triggered()
{
    //ui->tableView->hide();
    list<Meeting> meetings = agendaService_.listAllMeetings(userName_);
    if (!meetings.empty()) {
        printMeetings(meetings);
    }
    else {
        QMessageBox::information(NULL, "List Meetings", "Sorry, no meeting exist", QMessageBox::Ok);
        ui->tableView->hide();
    }
}


/**
 * @brief MainWindow::on_actionList_All_Sponsor_Meetings_triggered
 * slot to find out all the sponsor meeting of the log in user
 */
void MainWindow::on_actionList_All_Sponsor_Meetings_triggered()
{
    list<Meeting> meetings = agendaService_.listAllSponsorMeetings(userName_);
    if (!meetings.empty()) {
       printMeetings(meetings);
    }
    else {
        QMessageBox::information(NULL, "List Meetings", "Sorry, no sponsor meeting exist", QMessageBox::Ok);
        ui->tableView->hide();
    }
}


/**
 * @brief MainWindow::on_actionList_All_Participant_Meetings_triggered
 * slot to find out all participator meetings of the log in user
 */
void MainWindow::on_actionList_All_Participant_Meetings_triggered()
{
    //ui->tableView->hide();
    list<Meeting> meetings = agendaService_.listAllParticipateMeetings(userName_);
    if (!meetings.empty()) {
       printMeetings(meetings);
    }
    else {
        QMessageBox::information(NULL, "List Meetings", "Sorry, no participate meeting exist", QMessageBox::Ok);
        ui->tableView->hide();
    }
}


/**
 * @brief MainWindow::on_actionCreate_A_Meeting_triggered
 */
void MainWindow::on_actionCreate_A_Meeting_triggered()
{
    ui->tableView->hide();
    ui->welcomewidget->hide();
    ui->createmtwidget->show();
}


void MainWindow::on_crtMtBtn_clicked()
{

}


/**
 * @brief MainWindow::printMeetings
 * @param meetings
 * An assitant function to print the meetings
 */
void MainWindow::printMeetings(std::list<Meeting> meetings) {
    int rows = meetings.size();
    QStandardItemModel *model = new QStandardItemModel(rows, 5, this);
    model->setHorizontalHeaderItem(0, new QStandardItem(QString("Title")));
    model->setHorizontalHeaderItem(1, new QStandardItem(QString("Sponsor")));
    model->setHorizontalHeaderItem(2, new QStandardItem(QString("Participator")));
    model->setHorizontalHeaderItem(3, new QStandardItem(QString("Start Time")));
    model->setHorizontalHeaderItem(4, new QStandardItem(QString("End Time")));
    list<Meeting>::iterator it;
    int i;
    for (i = 0, it = meetings.begin(); it != meetings.end(); ++i, ++it) {
        QStandardItem *titleItem = new QStandardItem(QString(it->getTitle().c_str()));
        QStandardItem *sponsorItem = new QStandardItem(QString(it->getSponsor().c_str()));
        QStandardItem *partiItem = new QStandardItem(QString(it->getParticipator().c_str()));
        QStandardItem *startItem = new QStandardItem(QString(Date::dateToString(it->getStartDate()).c_str()));
        QStandardItem *endItem = new QStandardItem(QString(Date::dateToString(it->getEndDate()).c_str()));
        model->setItem(i, 0, titleItem);
        model->setItem(i, 1, sponsorItem);
        model->setItem(i, 2, partiItem);
        model->setItem(i, 3, startItem);
        model->setItem(i, 4, endItem);
    }
    ui->tableView->setModel(model);
    ui->tableView->show();
}






