#include "AgendaService.h"

using std::string;
using std::list;

AgendaService::AgendaService() { }

AgendaService::~AgendaService() {
    storage_->getInstance()->sync();
}


bool AgendaService::userLogIn(string userName, string password) {
    list<User> tempList;
    tempList = storage_->getInstance()->queryUser(
        [&userName, &password](const User &user) {
            return (user.getName() == userName) && 
                   (user.getPassword() == password);
        });
    if (!tempList.empty())
        return true;
    else
        return false;
}

bool AgendaService::userRegister(string userName, string password,
                                string email, string phone) {
    list<User> tempList;
    tempList = storage_->getInstance()->queryUser(
        [&userName](const User &user) {
            return user.getName() == userName;
        });
    //username already exist
    if (!tempList.empty()) 
        return false;

    storage_->getInstance()->createUser(
            User(userName, password, email, phone));
    return true;
}

//got to delete the meeting created by the user together
bool AgendaService::deleteUser(string userName, string password) {
    //0 for flase, and others for true
    return storage_->getInstance()->deleteUser(
        [&userName, &password](const User& user) {
            return (user.getName() == userName) &&
                   (user.getPassword() == password);
        }) && deleteAllMeetings(userName);
}

list<User> AgendaService::listAllUsers(void) {
    return storage_->getInstance()->queryUser(
        [](const User &user) {
            return user.getName() != "";
        });
}


bool AgendaService::createMeeting(string userName, string title,
                                  string participator,
                                  string startDate, string endDate) {

    Date startDateD = Date::stringToDate(startDate),
         endDateD = Date::stringToDate(endDate);

    //time and period should be valid
    if (!Date::isValid(startDateD) ||
        !Date::isValid(endDateD) ||
        startDateD >= endDateD)
        return false;

    //sponsor and participator should both exists
    list<User> sponsorList = storage_->getInstance()->queryUser(
        [&userName](const User &user) {
            return user.getName() == userName;
        });
    list<User> participatorList = storage_->getInstance()->queryUser(
        [&participator](const User &user) {
            return user.getName() == participator;
        });
    if (sponsorList.empty() || participator.empty())
        return false;

    //title should not be exist before
    list<Meeting> meetingList = storage_->getInstance()->queryMeeting(
        [&title](const Meeting &meeting) {
            return meeting.getTitle() == title;
        });
    if (!(meetingList.empty()))
        return false;

    //meeting should not exist in the time period before
    list<Meeting> sponsorMeeting = meetingQuery(userName, startDate, endDate);
    list<Meeting> participatorMeeting = meetingQuery(participator, startDate, endDate);
    if ( !(sponsorMeeting.empty() && participatorMeeting.empty()) )
        return false;

    //create meeting
    storage_->getInstance()->createMeeting(
        Meeting(userName, participator, startDateD, endDateD, title));

    return true;
}

//the username here can be sponsor or participator
list<Meeting> AgendaService::meetingQuery(string userName, string title) {
    return storage_->getInstance()->queryMeeting(
        [&userName, &title](const Meeting &meeting) {
            return ((meeting.getSponsor() == userName) || (meeting.getParticipator() == userName)) &&
                   (meeting.getTitle() == title);
        });
}

//the username here can be sponsor or participator
//the meetings between startDate and endDate are all valid
list<Meeting> AgendaService::meetingQuery(string userName, string startDate,
                                         string endDate) {
    return storage_->getInstance()->queryMeeting(
        [&userName, &startDate, &endDate](const Meeting &meeting) {
            return ((meeting.getSponsor() == userName) || (meeting.getParticipator() == userName)) &&
                   ((meeting.getStartDate() >= Date::stringToDate(startDate)) && 
                    (meeting.getEndDate() <= Date::stringToDate(endDate)));
        });
}

//the username here can be sponsor or participator
list<Meeting> AgendaService::listAllMeetings(string userName) {
    return storage_->getInstance()->queryMeeting(
        [&userName](const Meeting &meeting) {
            return (meeting.getSponsor() == userName) ||
                   (meeting.getParticipator() == userName);
        });    
}

list<Meeting> AgendaService::listAllSponsorMeetings(string userName) {
    return storage_->getInstance()->queryMeeting(
        [&userName](const Meeting &meeting) {
            return meeting.getSponsor() == userName;
        });
}

list<Meeting> AgendaService::listAllParticipateMeetings(string userName) {
    return storage_->getInstance()->queryMeeting(
        [&userName](const Meeting &meeting) {
            return meeting.getParticipator() == userName;
        });
}

bool AgendaService::deleteMeeting(string userName, string title) {
    list<Meeting> theMeeting = meetingQuery(userName, title);
    //if the meeting does not exist, make it as delete already
    if (theMeeting.empty()) 
        return true;

    return storage_->getInstance()->deleteMeeting(
        [&userName, &title](const Meeting &meeting) {
            return ((meeting.getSponsor() == userName) || (meeting.getParticipator() == userName)) &&
                   (meeting.getTitle() == title);
        });
}

bool AgendaService::deleteAllMeetings(string userName) {
    list<Meeting> allMeetings = listAllMeetings(userName);
    //if the user does not involve in any meetings
    if (allMeetings.empty())
        return true;

    return storage_->getInstance()->deleteMeeting(
        [&userName](const Meeting &meeting) {
            return (meeting.getParticipator() == userName) ||
                   (meeting.getSponsor() == userName);
        });
}

void AgendaService::startAgenda(void) {

}

void AgendaService::quitAgenda(void) {
    storage_->getInstance()->sync();
}