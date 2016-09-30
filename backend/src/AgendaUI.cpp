#include "AgendaUI.h"

#include <iomanip>      // std::setw

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::list;
using std::setw;

AgendaUI::AgendaUI() { }

void AgendaUI::OperationLoop(void) {
    userName_ = ""; userPassword_ = "";
    startAgenda();
    string option = getOperation();
    while (executeOperation(option)) 
        option = getOperation();
}


void AgendaUI::startAgenda(void) {
    cout << "------------------ Agenda ------------------" << endl
         << "Action:"                                      << endl
         << "l\t- log in Agenda by username and password"  << endl
         << "r\t- regist an Agenda account"                << endl
         << "q\t- quit Agenda"                             << endl
         << "--------------------------------------------" << endl;      
}

string AgendaUI::getOperation() {
    //no user login
    if (userName_ == "")
        cout << "Agenda : ~$ ";
    else
        cout << "\nAgenda@" << userName_ << " : # ";
    string option;
    cin >> option;
    cout << endl;
    return option;
}

bool AgendaUI::executeOperation(string op) {
    //no user login
    if (userName_ == "") {
        if (op == "l") {
            userLogIn();
        } else if (op == "r") {
            userRegister();
        } else if (op == "q") {
            quitAgenda();
            return false;
        } else {
            cout << "sorry, error command!" << endl;
        }
    }
    //after log in
    else {
        if (op == "o") {
            userLogOut();
            startAgenda();
        } else if (op == "dc") {
            deleteUser();
            startAgenda();
        } else if (op == "lu") {
            listAllUsers();
        } else if (op == "cm") {
            createMeeting();
        } else if (op == "la") {
            listAllMeetings();
        } else if (op == "las") {
            listAllSponsorMeetings();
        } else if (op == "lap") {
            listAllParticipateMeetings();
        } else if (op == "qm") {
            queryMeetingByTitle();
        } else if (op == "qt") {
            queryMeetingByTimeInterval();
        } else if (op == "dm") {
            deleteMeetingByTitle();
        } else if (op == "da") {
            deleteAllMeetings();
        } else {
            cout << "sorry, error command!" << endl;
        }       
    }
    return true;
}

void AgendaUI::userLogIn(void) {
    cout << "[log in] [username] [password]" << endl
         << "[log in] ";
    string username, password;
    cin >> username >> password;
    if (agendaService_.userLogIn(username, password)) {
        userName_ = username;
        userPassword_ = password;
        cout << "[log in] succeed!" << endl;
        //print menu
        cout << "------------------ Agenda ------------------" << endl
             << "Action:"                                      << endl
             << "o\t - log out Agenda"                         << endl
             << "dc\t - delete Agenda account"                 << endl
             << "lu\t - list all Agenda user"                  << endl
             << "cm\t - create a meeting"                      << endl
             << "la\t - list all meetings"                     << endl
             << "las\t - list all sponsor meetings"            << endl
             << "lap\t - list all participator meetings"       << endl
             << "qm\t - query meeting by title"                << endl
             << "qt\t - query meeting by time interval"        << endl
             << "dm\t - delete meeting by title"               << endl
             << "da\t - delete all meetings"                   << endl
             << "--------------------------------------------" << endl; 
    }
    else {
        cout << "[error] log in fail!" << endl;
    }
}

void AgendaUI::userRegister(void) {
    cout << "[register] [user name] [password] [email] [phone]" << endl
         << "[register] ";
    string username, password, email, phone;
    cin >> username >> password >> email >> phone;
    if (agendaService_.userRegister(username, password, email, phone)) {
        cout << "[register] succeed!" << endl;
    }
    else {
        cout << "[error] register failed!" << endl;
    }         
}

void AgendaUI::quitAgenda(void) {
    agendaService_.quitAgenda();
    cout << "\nGoodBye!" << endl;
}

void AgendaUI::userLogOut(void) {
    userName_ = ""; 
    userPassword_ = "";
}

void AgendaUI::deleteUser(void) {
    if (agendaService_.deleteUser(userName_, userPassword_)) {
        cout << "[delete Agenda account] succeed!" << endl;
        userName_ = ""; 
        userPassword_ = "";
    } 
    else {
        cout << "[error] delete Agenda account fail!" << endl;
    }
}

void AgendaUI::listAllUsers(void) {
    cout << "[list all users]" << endl << endl;
    list<User> users = agendaService_.listAllUsers();
    if (!users.empty()) {
        cout.flags(std::ios::left);
        cout << setw(8) << "name" 
             << setw(10) << "email" 
             << setw(10) << "phone" << endl;
        for (auto &user : users) {
            cout << setw(8) << user.getName()
                 << setw(10) << user.getEmail()
                 << setw(10) << user.getPhone() << endl;
        }
        cout.flags(std::ios::right);
    }
    else {
        cout << "Sorry, no user exist!" << endl;
    }
}

void AgendaUI::createMeeting(void) {
    cout << "[create meeting] [title] [participator] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl
         << "[create meeting] ";
    string title, participator, startDate, endDate;
    cin >> title >> participator >> startDate >> endDate;
    if (agendaService_.createMeeting(userName_, title, participator, startDate, endDate)) {
        cout << "[create meeting] succeed!" << endl;
    }
    else {
        cout << "[error] create meeting failed!" << endl;
    }
}

void AgendaUI::listAllMeetings(void) {
    cout << "[list all meetings]" << endl << endl;
    list<Meeting> meetings = agendaService_.listAllMeetings(userName_);
    if (!meetings.empty()) {
        printMeetings(meetings);
    }
    else {
        cout << "Sorry, no meeting exist" << endl;
    }
}

void AgendaUI::listAllSponsorMeetings(void) {
    cout << "[list all sponsor meetings]" << endl << endl;
    list<Meeting> meetings = agendaService_.listAllSponsorMeetings(userName_);
    if (!meetings.empty()) {
        printMeetings(meetings);
    }
    else {
        cout << "Sorry, no sponsor meeting exist" << endl;
    }
}

void AgendaUI::listAllParticipateMeetings(void) {
    cout << "[list all participator meetings]" << endl << endl;
    list<Meeting> meetings = agendaService_.listAllParticipateMeetings(userName_);
    if (!meetings.empty()) {
        printMeetings(meetings);
    }
    else {
        cout << "Sorry, no participator meeting exist" << endl;
    }    
}

void AgendaUI::queryMeetingByTitle(void) {
    cout << "[query meetings] [title]:" << endl
         << "[query meetings] ";
    string title;
    cin >> title;
    list<Meeting> meetings = agendaService_.meetingQuery(userName_, title);
    if (!meetings.empty()) {
        cout.flags(std::ios::left);
        cout << setw(14) << "sponsor"
             << setw(14) << "participator"
             << setw(18) << "start time"
             << setw(18) << "end time" << endl;
        for (auto& meeting : meetings) {
            cout << setw(14) << meeting.getSponsor()
                 << setw(14) << meeting.getParticipator() 
                 << setw(18) << Date::dateToString(meeting.getStartDate()) 
                 << setw(18) << Date::dateToString(meeting.getEndDate()) << endl;
        }
        cout.flags(std::ios::right);
    }
    else {
        cout << "Sorry, no existing meeting titled as " << title << endl; 
    }
}

void AgendaUI::queryMeetingByTimeInterval(void) {
    cout << "[query meetings] [start time(yyyy-mm-dd/hh:mm)] [end time(yyyy-mm-dd/hh:mm)]" << endl
         << "[query meetings] ";
    string startTime, endTime;
    cin >> startTime >> endTime;
    list<Meeting> meetings = agendaService_.meetingQuery(userName_, startTime, endTime);
    if (!meetings.empty()) {
        cout << "\n[query meetings]" << endl;
        printMeetings(meetings);
    }
    else {
        cout << "Sorry, no meeting exist during " << startTime << "~" << endTime << endl; 
    }  
}

void AgendaUI::deleteMeetingByTitle(void) {
    cout << "[delete meeting] [title]" << endl
         << "[delete meeting] ";
    string title;
    cin >> title;
    if (agendaService_.deleteMeeting(userName_, title)) {
        cout << "\n[delete meeting by title] succeed!" << endl; 
    }
    else {
        cout << "[error] delete meeting fail!" << endl;
    }
}

void AgendaUI::deleteAllMeetings(void) {
    if (agendaService_.deleteAllMeetings(userName_)) {
        cout << "\n[delete all meetings] succeed!" << endl;
    }
    else {
        cout << "[error] delete all meetings fail!" << endl;
    }
}

void AgendaUI::printMeetings(list<Meeting> meetings) {
    cout.flags(std::ios::left);
    cout << setw(10) << "title" 
         << setw(14) << "sponsor"
         << setw(14) << "participator"
         << setw(18) << "start time"
         << setw(18) << "end time" << endl;
    for (auto& meeting : meetings) {
        cout << setw(10) << meeting.getTitle() 
             << setw(14) << meeting.getSponsor() 
             << setw(14) << meeting.getParticipator()
             << setw(18) << Date::dateToString(meeting.getStartDate())
             << setw(18) << Date::dateToString(meeting.getEndDate()) << endl;
    }
    cout.flags(std::ios::right);
}