#include "Storage.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>

using std::string;
using std::map;
using std::endl;
using std::list;

//definition, fix undefine reference
Storage* Storage::instance_;

Storage::Storage() {
    readFromFile("data/Agenda.json");
}

bool Storage::sync(void) {
    return  writeToFile("data/Agenda.json");
}

void lineTokenizer(string &line, map<string, string> &nameToValue) {
    //remove { }
    line = line.substr(1, line.size() - 2);
    std::istringstream lineStream(line);
    string words, name, value;
    //get each token, and separate them into name and value
    while (getline(lineStream, words, ',')) {
        string tempWord;
        size_t cnt = 0;
        for (; cnt < words.size(); ++cnt) {
            //the 1st ':' should be the separtor, and the date have ':'....
            if (words[cnt] == ':') {
                name = tempWord;
                break;
            }
            else {
                tempWord += words[cnt];
            }
        }
        //get value and also remove "", 3 means " + : + "
        value = words.substr(cnt + 2, words.size() - name.size() - 3);
        nameToValue[name] = value;
    }
}

bool Storage::readFromFile(const char *fpath) {
    std::ifstream infile(fpath);
    if (!infile)
        return false;

    string line, numStr;
    int num = 0;

    //restore userList from file
    getline(infile, line);
    for (size_t i = 0; i < line.size(); i++) {
        if (std::isdigit(line[i]))
            numStr += line[i];
    }
    if (!numStr.empty())
        num = std::stoi(numStr);
    for (int i = 0; i < num; i++) {
        map<string, string> nameToValue;
        getline(infile, line);
        lineTokenizer(line, nameToValue);
        userList_.push_back(
            User(nameToValue["name"], nameToValue["password"],
                nameToValue["email"], nameToValue["phone"]));
    }

    //restore meetingList from file
    getline(infile, line);
    numStr.clear(); num = 0;
    for (size_t i = 0; i < line.size(); i++) {
        if (std::isdigit(line[i]))
            numStr += line[i];
    }
    if (!numStr.empty())
        num = std::stoi(numStr);
    for (int i = 0; i < num; i++) {
        map<string, string> nameToValue;
        getline(infile, line);
        lineTokenizer(line, nameToValue);
        meetingList_.push_back(
            Meeting(nameToValue["sponsor"], nameToValue["participator"],
                    Date::stringToDate(nameToValue["sdate"]),
                    Date::stringToDate(nameToValue["edate"]),
                    nameToValue["title"]));
    }

    return true;
}


bool Storage::writeToFile(const char *fpath) {
    std::ofstream outfile(fpath, std::ofstream::trunc);
    if (!outfile)
        return false;

    outfile << "{collection:\"User\",total:" << userList_.size() << "}" << endl;
    for (auto &user : userList_) {
        outfile << "{name:\"" << user.getName() << "\","
                << "password:\"" << user.getPassword() << "\","
                << "email:\"" << user.getEmail() << "\","
                << "phone:\"" << user.getPhone() << "\"}" << endl;
    }
    outfile << "{collection:\"Meeting\",total:" << meetingList_.size() << "}" << endl;
    for (auto &meeting : meetingList_) {
        outfile << "{sponsor:\"" << meeting.getSponsor() << "\","
                << "participator:\"" << meeting.getParticipator() << "\","
                << "sdate:\"" << Date::dateToString(meeting.getStartDate()) << "\","
                << "edate:\"" << Date::dateToString(meeting.getEndDate()) << "\","
                << "title:\"" << meeting.getTitle() << "\"}" << endl;
    }

    return true;
}

Storage* Storage::getInstance(void) {
    if (instance_ == NULL)
        instance_ = new Storage();
    return instance_;
}

Storage::~Storage() {
    if (instance_ != NULL)
        delete instance_;
}

void Storage::createUser(const User &user) {
    userList_.push_back(user);
}

list<User> Storage::queryUser(std::function<bool(const User&)> filter) {
    list<User> tempList;
    for (auto &user : userList_) {
        if (filter(user)) {
            tempList.push_back(user);
        }
    }
    return tempList;
}

int Storage::updateUser(std::function<bool(const User&)> filter,
        std::function<void(User&)> switcher) {
    int cnt = 0;
    for (auto &user : userList_) {
        if (filter(user)) {
            switcher(user);
            ++cnt;
        }
    }
    return cnt;
}

int Storage::deleteUser(std::function<bool(const User&)> filter) {
    int cnt = 0;
    auto it = userList_.begin();
    while (it != userList_.end()) {
        if (filter(*it)) {
            it = userList_.erase(it);
            ++cnt;
        }
        else {
            ++it;
        }
    }
    return cnt;
}

void Storage::createMeeting(const Meeting &meeting) {
    meetingList_.push_back(meeting);
}

list<Meeting> Storage::queryMeeting(std::function<bool(const Meeting&)> filter) {
    list<Meeting> tempList;
    for (auto &meeting : meetingList_) {
        if (filter(meeting))
            tempList.push_back(meeting);
    }
    return tempList;
}

int Storage::updateMeeting(std::function<bool(const Meeting&)> filter,
                           std::function<void(Meeting&)> switcher) {
    int cnt = 0;
    for (auto &meeting : meetingList_) {
        if (filter(meeting)) {
            switcher(meeting);
            ++cnt;
        }
    }
    return cnt;
}

int Storage::deleteMeeting(std::function<bool(const Meeting&)> filter) {
    int cnt = 0;
    auto it = meetingList_.begin();
    while(it != meetingList_.end()) {
        if (filter(*it)) {
            it = meetingList_.erase(it);
            ++cnt;
        }
        else {
            ++it;
        }
    }
    return cnt;
}
