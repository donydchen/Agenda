#include "SqliteService.h"
#include <fstream>
#include <cstdio>

SqliteService::SqliteService() {
    // create db and table if needed
    const char* dbPath = "Agenda.sqlite3";
    int rc = sqlite3_open(dbPath, &db);
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
    sql = "SELECT * FROM User WHERE name = \'" + userName
           + "\' and password = \'" + password + "\';";
    return !isResEmpty();
}


bool SqliteService::userRegister(string userName, string password,
            string email, string phone) {
    sql = "SELECT * FROM User WHERE name = \'" + userName + "\';";
    if (!isResEmpty())
        return false;

    fprintf(sql, "INSERT INTO User (name, password, email, phone)" \
                 "VALUES (\'%s\', \'%s\', \'%s\', \'%s\');",
                 userName, password, email, phone);
    execSQL();
    return true;
}


SqliteService::initDB() {
    // create table user if not exist
    sql = "CREATE TABLE IF NOT EXISTS User (" \
          "name     TEXT  NOT NULL, " \
          "password TEXT  NOT NULL, " \
          "email    TEXT  NOT NULL, " \
          "phone    TEXT  NOT NULL);";
    execSQL();
    // create table meeting if not exist
    sql = "CREATE TABLE IF NOT EXISTS Meeting (" \
          "sponsor      TEXT  NOT NULL, " \
          "participator TEXT  NOT NULL, " \
          "sdate        TEXT  NOT NULL, " \
          "edate        TEXT  NOT NULL, " \
          "title        TEXT  NOT NULL);";
    execSQL();
}


/*
 * Execute the current UPDATE sql statement.
 */
void SqliteService::execSQL() {
    rc = sqlite3_exec(db, sql.c_str(), NULL, 0, NULL);
    if (rc != SQLITE_OK)
        throw string(sqlite3_errmsg(db));
}

/*
 * Execute the current QUERY sql statement and
 * Check whether the result is empty
 */
bool SqliteService::isResEmpty(void) {
    sqlite3_stmt* stmt;
    bool ans = false;

    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, NULL);
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
