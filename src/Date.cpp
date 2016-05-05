#include "Date.h"

#include <vector>
#include <iostream>

using std::vector;
using std::string;

inline bool isLeapYear(int y) {
    return ( ((y % 4 == 0) && (y % 100 != 0)) || (y % 400 == 0) );
}

/*int strToInt(const std::string &str) {
    int answer = 0;
    std::string::size_type start = 0, end = str.size() - 1;
    while (str[start] == '0') start++;
    int num = 1;
    for (; end >= start; --end) {
        answer += (str[end] - '0') * num;
        num *= 10;
    }
    return answer;
}*/

inline string intToStr(int num) {
    string str;
    if (num == 0) 
        str = "00";
    else if (num < 10)
        str = ("0" + std::to_string(num));
    else
        str = std::to_string(num);
    return str;
}

Date::Date() : year_(2016), month_(5), day_(4),
            hour_(0), minute_(0) { }

Date::Date(int y, int m, int d, int h, int mi) : 
    year_(y), month_(m), day_(d), hour_(h), minute_(m) { }

int Date::getYear(void) const {
    return year_;
}

void Date::setYear(int year) {
    year_ = year;
}

int Date::getMonth(void) const {
    return month_;
}

void Date::setMonth(int month) {
    month_ = month;
}

int Date::getDay(void) const {
    return day_;
}

void Date::setDay(int day) {
    day_ = day;
}

int Date::getHour(void) const {
    return hour_;
}

void Date::setHour(int hour) {
    hour_ = hour;
}

int Date::getMinute(void) const {
    return minute_;
}

void Date::setMinute(int minute) {
    minute_ = minute;
}

//maybe const & will be better
bool Date::isValid(Date date) {
    static vector<int> month_day = 
        {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if ( !(date.minute_ >= 0 && date.minute_ <= 60) ) return false;
    if ( !(date.hour_ >= 0 && date.hour_ <= 24) ) return false;
    if ( !(date.month_ >= 1 && date.month_ <= 12) ) return false;
    //decide day invalid
    if ( isLeapYear(date.year_) && date.month_ == 2) {
        if ( !(date.day_ >= 1 && date.day_ <= 29 ) )
            return false;
    } else {
        if ( !(date.day_ >= 1 && date.day_ <= month_day[date.month_ - 1]) )
            return false;
    }

    return true;
}

Date Date::stringToDate(std::string dateString) {
    vector<int> dateInt;
    string temp = "";
    //there is only one punctuation between numbers, that's why we can do like this.
    for (size_t i = 0; i < dateString.length(); ++i) {
        if (dateString[i] >= '0' && dateString[i] <= '9') {
            temp += dateString[i];
        }
        //it is punctuation
        else {
            dateInt.push_back(std::stoi(temp));
            temp.clear();
        }
    }
    //deal with minute
    dateInt.push_back(std::stoi(temp));
    if (dateInt.size() != 5) {
        std::cout << "The String is in error type" << std::endl;
        return Date();
    }
    return Date(dateInt[0], dateInt[1], dateInt[2], dateInt[3], dateInt[4]);
}

std::string Date::dateToString(Date date) {
    return std::to_string(date.year_) + "-" + 
            intToStr(date.month_) + "-" +
            intToStr(date.day_) + "/" +
            intToStr(date.hour_) + ":" +
            intToStr(date.minute_);
}

Date& Date::operator=(const Date& date) {
    year_ = date.year_;
    month_ = date.month_;
    day_ = date.day_;
    hour_ = date.hour_;
    minute_ = date.minute_;
    return *this;
}

bool Date::operator==(const Date& date) const {
    return  (year_ == date.year_) &&
            (month_ == date.month_) &&
            (day_ == date.day_) &&
            (hour_ == date.hour_) &&
            (minute_ == date.minute_);
}

bool Date::operator>(const Date& date) const {
    if (year_ > date.year_) {
        return true;
    }
    else if (year_ == date.year_) {
        if (month_ > date.month_) {
            return true;
        }
        else if (month_ == date.month_) {
            if (day_ > date.day_) {
                return true;
            }
            else if (day_ == date.day_) {
                if (hour_ > date.hour_) {
                    return true;
                }
                else if (hour_ == date.hour_) {
                    if (minute_ > date.minute_) {
                        return true;
                    }
                }
            }
        }
    }  
    return false;
}

bool Date::operator<(const Date& date) const {
    return !((*this) >= date);
}

bool Date::operator>=(const Date& date) const {
    return (*this) > date || (*this) == date;
}

bool Date::operator<=(const Date& date) const {
    return !((*this) > date);
}