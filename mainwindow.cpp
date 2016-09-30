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

    btnStatus = BtnStatus::Query;

    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    showPage(PageType::HomePage);
}

MainWindow::~MainWindow()
{
    agendaService_.quitAgenda();
    delete ui;
}


/**
 * @brief MainWindow::on_actionHome_triggered
 * go to homepage
 */
void MainWindow::on_actionHome_triggered()
{
    showPage(PageType::HomePage);
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
        showPage(PageType::HomePage);
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
        showPage(PageType::HomePage);
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
        showPage(HomePage);
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
        showPage(PageType::HomePage);
    }
}


/**
 * @brief MainWindow::on_actionCreate_A_Meeting_triggered
 */
void MainWindow::on_actionCreate_A_Meeting_triggered()
{
    ui->tableView->hide();
    ui->welcomewidget->hide();

    // add users to participator combo box
    list<User> users = agendaService_.listAllUsers();
    list<User>::iterator it;
    for (it = users.begin(); it != users.end(); ++it) {
        string user = it->getName();
        if (user != userName_)
            ui->parComboBox->addItem(QString::fromStdString(user));
    }
    // set up date time picker yyyy-mm-dd/hh:mm
    ui->startDT->setCalendarPopup(true);
    ui->startDT->setDateTime(QDateTime::currentDateTime());
    ui->startDT->setDisplayFormat("MMMM dd,yyyy   hh:mm");
    ui->endDT->setCalendarPopup(true);
    ui->endDT->setDateTime(QDateTime::currentDateTime());
    ui->endDT->setDisplayFormat("MMMM dd,yyyy   hh:mm");

    // show the widget
    ui->createmtwidget->show();

}


/**
 * @brief MainWindow::on_crtMtBtn_clicked
 * create a meeting
 */
void MainWindow::on_crtMtBtn_clicked()
{
    string title = ui->mtTitle->text().toUtf8().constData();
    string participator = ui->parComboBox->currentText().toUtf8().constData();
    string startTime = ui->startDT->dateTime().toString("yyyy-MM-dd/hh:mm").toUtf8().constData();
    string endTime = ui->endDT->dateTime().toString("yyyy-MM-dd/hh:mm").toUtf8().constData();
    if (agendaService_.createMeeting(userName_, title, participator, startTime, endTime)) {
        QMessageBox::information(NULL, "Create Meeting", "Successfully create a meeting!",
                                 QMessageBox::Ok);
        showPage(PageType::HomePage);
    }
    else {
        QMessageBox::critical(NULL, "Create Meeting", "Sorry, failed to create the meeeting!");
    }
}


/**
 * @brief MainWindow::on_actionDelete_A_Meeting_triggered
 * delete a meeting accroding to the given name
 */
void MainWindow::on_actionDelete_A_Meeting_triggered()
{
    showPage(PageType::SearchTitle);
    btnStatus = BtnStatus::Delete;
}


/**
 * @brief MainWindow::on_actionDelete_All_Meetings_triggered
 * Delete all the meeting connect to the log in user
 */
void MainWindow::on_actionDelete_All_Meetings_triggered()
{
    int ans = QMessageBox::question(NULL, "Delete Meetings",
                                    "Do you really want to delete ALL the meetings?",
                                    QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (ans == QMessageBox::Yes) {
        if (agendaService_.deleteAllMeetings(userName_)) {
            QMessageBox::information(NULL, "Delete Meetings", "Successfully deleted all the meeting!");
        }
        else {
            QMessageBox::critical(NULL, "Delete Meetings", "Failed to delete all meetings:(");
        }
    }
}


/**
 * @brief MainWindow::on_actionQuery_Meeting_By_Title_triggered
 */
void MainWindow::on_actionQuery_Meeting_By_Title_triggered()
{
    showPage(PageType::SearchTitle);
    btnStatus = BtnStatus::Query;
}


/**
 * @brief MainWindow::on_actionQuery_Meeting_By_Time_Interval_triggered
 * query meeting exist in the provided time interval
 */
void MainWindow::on_actionQuery_Meeting_By_Time_Interval_triggered()
{
    // set up date time picker yyyy-mm-dd/hh:mm
    ui->startDT_2->setCalendarPopup(true);
    ui->startDT_2->setDateTime(QDateTime::currentDateTime());
    ui->startDT_2->setDisplayFormat("MMMM dd,yyyy   hh:mm");
    ui->endDT_2->setCalendarPopup(true);
    ui->endDT_2->setDateTime(QDateTime::currentDateTime());
    ui->endDT_2->setDisplayFormat("MMMM dd,yyyy   hh:mm");
    //show the time interval search widget
    showPage(PageType::SearchTime);
}


/**
 * @brief MainWindow::on_searchBtn_clicked
 * to search a meeting according to the title
 */
void MainWindow::on_searchBtn_clicked()
{
    string title = ui->searchTitle->text().toUtf8().constData();
    //search the meeting anyhow
    list<Meeting> meetings = agendaService_.meetingQuery(userName_, title);
    if (meetings.empty()) {
        QMessageBox::critical(NULL, "No Meeting", "Sorry but no such meeting");
    }
    else {
        // if it is to delete a meeting by its title
        if (btnStatus == BtnStatus::Delete) {
            string content = "Do you really want to delete meeting \"" + title + "\"?";
            int ans = QMessageBox::question(NULL, "Delete Meeting", content.c_str(),
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
            if (ans == QMessageBox::Yes) {
                if (agendaService_.deleteMeeting(userName_, title)) {
                    QMessageBox::information(NULL, "Delete Meeting",
                                             "Successfully delete the meetings!");
                    on_actionHome_triggered();
                }
                else {
                    QMessageBox::critical(NULL, "Delete Meeting", "Failed to delete the meeting!");
                }
            }
        }
        //if it is to query a meeting by title
        else if (btnStatus == BtnStatus::Query) {
            //show the meeting in the tableView
            int rows = meetings.size();
            QStandardItemModel *model = new QStandardItemModel(rows, 4, this);
            model->setHorizontalHeaderItem(0, new QStandardItem(QString("Sponsor")));
            model->setHorizontalHeaderItem(1, new QStandardItem(QString("Participator")));
            model->setHorizontalHeaderItem(2, new QStandardItem(QString("Start Time")));
            model->setHorizontalHeaderItem(3, new QStandardItem(QString("End Time")));
            list<Meeting>::iterator it;
            int i;
            for (i = 0, it = meetings.begin(); it != meetings.end(); ++i, ++it) {
                QStandardItem *sponsorItem = new QStandardItem(QString(it->getSponsor().c_str()));
                QStandardItem *partiItem = new QStandardItem(QString(it->getParticipator().c_str()));
                QStandardItem *startItem = new QStandardItem(QString(Date::dateToString(it->getStartDate()).c_str()));
                QStandardItem *endItem = new QStandardItem(QString(Date::dateToString(it->getEndDate()).c_str()));
                model->setItem(i, 0, sponsorItem);
                model->setItem(i, 1, partiItem);
                model->setItem(i, 2, startItem);
                model->setItem(i, 3, endItem);
            }
            ui->tableView->setModel(model);
            showPage(PageType::TableView);
        }
        else {
            //do nothing
        }
    }
}


/**
 * @brief MainWindow::on_searchBtn_2_clicked
 * to search meeting according to the provided time interval
 */
void MainWindow::on_searchBtn_2_clicked()
{
    string startTime = ui->startDT_2->dateTime().toString("yyyy-MM-dd/hh:mm").toUtf8().constData();
    string endTime = ui->endDT_2->dateTime().toString("yyyy-MM-dd/hh:mm").toUtf8().constData();
    list<Meeting> meetings = agendaService_.meetingQuery(userName_, startTime, endTime);
    if (!meetings.empty()) {
        printMeetings(meetings);
    }
    else {
        string content = "Sorry, no meeting exist during <br>" + startTime + " ~ " + endTime;
        QMessageBox::warning(NULL, "Query Meetings", content.c_str());
    }
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
    showPage(PageType::TableView);
}


/**
 * @brief MainWindow::showPage
 * Hide all other widget and show the widget according to the pageType
 */
void MainWindow::showPage(PageType pageType) {
    ui->tableView->hide();
    ui->createmtwidget->hide();
    ui->searchwidget->hide();
    ui->searchwidget_2->hide();
    ui->welcomewidget->hide();
    switch(pageType) {
        case PageType::HomePage:
            ui->welcomewidget->show(); break;
        case PageType::TableView:
            ui->tableView->show(); break;
        case PageType::CreateMT:
            ui->createmtwidget->show(); break;
        case PageType::SearchTitle:
            ui->searchwidget->show(); break;
        case PageType::SearchTime:
            ui->searchwidget_2->show(); break;
        default:
            ui->welcomewidget->show();
    }
}

