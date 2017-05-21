#ifndef JSONSERVICE_H
#define JSONSERVICE_H

#include "AgendaService.h"
#include "Storage.h"
using std::list;
using std::string;

class JsonService: public AgendaService {
public:
    JsonService();
    ~JsonService();
    // User operation
    virtual bool userLogIn(string userName, string password);
    virtual bool userRegister(string userName, string password,
                      string email, string phone);
    virtual bool deleteUser(string userName, string password);
    virtual list<User> listAllUsers(void) const;
    // Meeting operation
    virtual bool createMeeting(string userName, string title,
                       string participator,
                       string startDate, string endDate);
    virtual list<Meeting> meetingQuery(string userName, string title) const;
    virtual list<Meeting> meetingQuery(string userName, string startDate,
                               string endDate) const;
    virtual list<Meeting> listAllMeetings(string userName) const;
    virtual list<Meeting> listAllSponsorMeetings(string userName) const;
    virtual list<Meeting> listAllParticipateMeetings(string userName) const;
    virtual bool deleteMeeting(string userName, string title);
    virtual bool deleteAllMeetings(string userName);
private:
    Storage *storage_;
};


#endif
