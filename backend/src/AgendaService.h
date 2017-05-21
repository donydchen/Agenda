#ifndef AGENDASERVICE_H
#define AGENDASERVICE_H

#include "User.h"
#include "Meeting.h"
#include <list>
#include <string>

class AgendaService {
public:
    virtual bool userLogIn(std::string userName, std::string password) = 0;
    virtual bool userRegister(std::string userName, std::string password,
                      std::string email, std::string phone) = 0;
    virtual bool deleteUser(std::string userName, std::string password) = 0;
  	// a user can only delete itself
    virtual std::list<User> listAllUsers(void) const = 0;

    virtual bool createMeeting(std::string userName, std::string title,
                       std::string participator,
                       std::string startDate, std::string endDate) = 0;
    virtual std::list<Meeting> meetingQuery(std::string userName, std::string title) const = 0;
    virtual std::list<Meeting> meetingQuery(std::string userName, std::string startDate,
                                    std::string endDate) const = 0;
    virtual std::list<Meeting> listAllMeetings(std::string userName) const = 0;
    virtual std::list<Meeting> listAllSponsorMeetings(std::string userName) const = 0;
    virtual std::list<Meeting> listAllParticipateMeetings(std::string userName) const = 0;
    virtual bool deleteMeeting(std::string userName, std::string title) = 0;
    virtual bool deleteAllMeetings(std::string userName) = 0;
};

#endif
