#ifndef SQLITE_SERVICE_H
#define SQLITE_SERVICE_H

#include "AgendaService.h"
#include "Storage.h"
#include <sqlite3.h>
using std::list;
using std::string;

class SqliteService: public AgendaService {
public:
    SqliteService();
    ~SqliteService();
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
    sqlite3* db;
    string sql;
    int  rc;
    bool initDB(void);
    void execSQL(void);
    bool isResEmpty(void);
};


#endif
