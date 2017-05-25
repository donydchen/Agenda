#include "SqliteService.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <iostream>
using std::cout;
using std::cin;
using std::endl;

SqliteService::SqliteService(string dbPath) : dbPath_(dbPath.c_str()) {
    serviceName_ = "SQLite";
    // create db and table if needed
    //const char* dbPath = "data/Agenda.sqlite3";
    int rc = sqlite3_open(dbPath_, &db);
    if (rc) {
        string errmsg(sqlite3_errmsg(db));
        errmsg = "Can't open database: " + errmsg;
        throw errmsg;
    }
    initDB();
}


SqliteService::~SqliteService() {
    sqlite3_close(db);
}


bool SqliteService::userLogIn(string userName, string password) {
    char sql [200];
    sprintf(sql, "SELECT * FROM User WHERE name = \'%s\' and " \
                 "password = \'%s\';", userName.c_str(), password.c_str());
    return !isResEmpty((const char *)sql);
}


bool SqliteService::userRegister(string userName, string password,
            string email, string phone) {
    //cout << "userRegister: " << userName << endl;
    char sql [200];
    sprintf(sql, "INSERT INTO User (name, password, email, phone) " \
                 "VALUES (\'%s\', \'%s\', \'%s\', \'%s\');",
                 userName.c_str(), password.c_str(), email.c_str(), phone.c_str());
    //cout << sql << endl;
    return execSQL((const char *)sql);
}


bool SqliteService::deleteUser(string userName, string password) {
    char sql [200];
    sprintf(sql, "DELETE FROM User WHERE name=\'%s\' and password=\'%s\';",
                 userName.c_str(), password.c_str());
    if (execSQL((const char *)sql) && deleteAllMeetings(userName))
        return true;
    return false;
}


list<User> SqliteService::listAllUsers(void) const{
    list<User> users;
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, "SELECT * FROM User;", -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        throw string(sqlite3_errmsg(db));
    }

    for(;;) {
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_DONE)
            break;
        if (rc != SQLITE_ROW) {
            string errmsg(sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            throw errmsg;
        }

        User user;
        user.setName(string((const char *)sqlite3_column_text(stmt, 0)));
        user.setPassword(string((const char *)sqlite3_column_text(stmt, 1)));
        user.setEmail(string((const char *)sqlite3_column_text(stmt, 2)));
        user.setPhone(string((const char *)sqlite3_column_text(stmt, 3)));
        users.push_back(user);
    }

    sqlite3_finalize(stmt);
    return users;
}


bool SqliteService::createMeeting(string userName, string title, string participator,
                                 string startDate, string endDate) {
    std::size_t startT = tsToSec(startDate.c_str());
    std::size_t endT = tsToSec(endDate.c_str());
    // time period should be valid
    if (startT >= endT)
        return false;

    char sql [200];
    sprintf(sql, "INSERT INTO Meeting (sponsor, participator, sdate, edate, title) " \
                 "VALUES (\'%s\', \'%s\', %d, %d, \'%s\');",
                 userName.c_str(), participator.c_str(), (int)startT, (int)endT, title.c_str());
    execSQL((const char *)sql);
    return true;
}


list<Meeting> SqliteService::meetingQuery(string userName, string title) const {
    char sql [200];
    sprintf(sql, "SELECT * FROM Meeting WHERE sponsor=\'%s\' AND title=\'%s\';",
                 userName.c_str(), title.c_str());
    return execSQLMeeting((const char *)sql);
}


list<Meeting> SqliteService::meetingQuery(string userName, string startDate, string endDate) const {
    std::size_t startT = tsToSec(startDate.c_str());
    std::size_t endT = tsToSec(endDate.c_str());
    char sql [200];
    sprintf(sql, "SELECT * FROM Meeting WHERE sponsor = \'%s\' " \
                 "AND sdate >= %d AND edate <= %d;",
                 userName.c_str(), (int)startT, (int)endT);
    return execSQLMeeting((const char *)sql);
}


list<Meeting> SqliteService::listAllMeetings(string userName) const {
    char sql [200];
    sprintf(sql, "SELECT * FROM Meeting WHERE (sponsor = \'%s\') " \
                 "OR (participator = \'%s\');", userName.c_str(), userName.c_str());
    return execSQLMeeting((const char *)sql);
}


list<Meeting> SqliteService::listAllSponsorMeetings(string userName) const {
    char sql [200];
    //cout << "listAllSponsorMeetings: " << userName << endl;
    sprintf(sql, "SELECT * FROM Meeting WHERE (sponsor = \'%s\');", userName.c_str());
    //cout << sql << endl;
    return execSQLMeeting((const char *)sql);
}


list<Meeting> SqliteService::listAllParticipateMeetings(string userName) const {
    char sql [200];
    sprintf(sql, "SELECT * FROM Meeting WHERE participator = \'%s\';",
                 userName.c_str());
    return execSQLMeeting((const char *)sql);
}


bool SqliteService::deleteMeeting(string userName, string title) {
    char sql [200];
    sprintf(sql, "DELETE FROM Meeting " \
                 "WHERE (sponsor = \'%s\' OR participator = \'%s\') " \
                 "AND (title = \'%s\');",
                 userName.c_str(), userName.c_str(), title.c_str());
    return execSQL((const char *)sql);
}


bool SqliteService::deleteAllMeetings(string userName) {
    char sql [200];
    sprintf(sql, "DELETE FROM Meeting " \
                 "WHERE (sponsor = \'%s\' OR participator = \'%s\');",
                 userName.c_str(), userName.c_str());
    return execSQL((const char *)sql);
}

void SqliteService::initDB() {
    string sql;
    // create table user if not exist
    sql = "CREATE TABLE IF NOT EXISTS User (" \
          "name     TEXT  PRIMARY KEY  NOT NULL, " \
          "password TEXT  NOT NULL, " \
          "email    TEXT  NOT NULL, " \
          "phone    TEXT  NOT NULL);";
    execSQL(sql.c_str());
    // create table meeting if not exist
    sql = "CREATE TABLE IF NOT EXISTS Meeting (" \
          "sponsor      TEXT     NOT NULL, " \
          "participator TEXT     NOT NULL, " \
          "sdate        INTEGER  NOT NULL, " \
          "edate        INTEGER  NOT NULL, " \
          "title        TEXT     PRIMARY KEY  NOT NULL);";
    execSQL(sql.c_str());
}


/*
 * Execute the current UPDATE sql statement.
 */
bool SqliteService::execSQL(const char* sql) const {
    int rc = sqlite3_exec(db, sql, NULL, 0, NULL);
    if (rc != SQLITE_OK)
        return false;
        //throw string(sqlite3_errmsg(db));
    return true;
}

/*
 * Execute the current QUERY sql statement and
 * Check whether the result is empty
 */
bool SqliteService::isResEmpty(const char* sql) const {
    sqlite3_stmt* stmt;
    bool ans = false;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
        throw string(sqlite3_errmsg(db));

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE && rc != SQLITE_ROW) {
        string errmsg(sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        throw errmsg;
    }

    if (rc == SQLITE_DONE || sqlite3_column_type(stmt, 0) == SQLITE_NULL)
        ans = true;

    sqlite3_finalize(stmt);
    return ans;
}


/*
 * Execute SQL statement, return the result as a list of Meeting object
 */
list<Meeting> SqliteService::execSQLMeeting(const char* sql) const {
    list<Meeting> meetings;
    sqlite3_stmt* stmt;
    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK) {
        throw string(sqlite3_errmsg(db));
    }

    for(;;) {
        rc = sqlite3_step(stmt);
        if (rc == SQLITE_DONE)
            break;
        if (rc != SQLITE_ROW) {
            string errmsg(sqlite3_errmsg(db));
            sqlite3_finalize(stmt);
            throw errmsg;
        }

        Meeting meeting;
        meeting.setSponsor(string((const char *)sqlite3_column_text(stmt, 0)));
        meeting.setParticipator(string((const char *)sqlite3_column_text(stmt, 1)));
        meeting.setStartDate(Date::stringToDate(secToTs(atoi((const char*)sqlite3_column_text(stmt, 2)))));
        meeting.setEndDate(Date::stringToDate(secToTs(atoi((const char*)sqlite3_column_text(stmt, 3)))));
        meeting.setTitle(string((const char *)sqlite3_column_text(stmt, 4)));
        meetings.push_back(meeting);
    }

    sqlite3_finalize(stmt);
    return meetings;
}

/*
 * Convert readable timestamp to unix time for storage
 */
std::size_t SqliteService::tsToSec(const char *timestamp) const {
    std::tm tm_struct;
    strptime(timestamp, "%Y-%m-%d/%H:%M", &tm_struct);
    tm_struct.tm_isdst = 0;
    std::size_t t = std::mktime(&tm_struct);

    return t;
}

/*
 * Convert unix time to readable timestamp for display
 */
string SqliteService::secToTs(int unixTime) const {
    std::time_t t = unixTime;
    std::tm *tm = std::localtime(&t);
    char dateTime[50];
    strftime(dateTime, sizeof(dateTime), "%Y-%m-%d/%H:%M", tm);
    return dateTime;
}
