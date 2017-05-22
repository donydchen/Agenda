#include "Meeting.h"

using std::string;


Meeting::Meeting() {}

Meeting::Meeting(string sponsor, string participator,
                Date startTime, Date endTime, string title) :
    sponsor_(sponsor),
    participator_(participator),
    startDate_(startTime),
    endDate_(endTime),
    title_(title) { }

string Meeting::getSponsor(void) const {
    return sponsor_;
}

void Meeting::setSponsor(string sponsor) {
    sponsor_ = sponsor;
}

string Meeting::getParticipator(void) const {
    return participator_;
}

void Meeting::setParticipator(string participator) {
    participator_ = participator;
}

Date Meeting::getStartDate(void) const {
    return startDate_;
}

void Meeting::setStartDate(Date startTime) {
    startDate_ = startTime;
}

Date Meeting::getEndDate(void) const {
    return endDate_;
}

void Meeting::setEndDate(Date endTime) {
    endDate_ = endTime;
}

string Meeting::getTitle(void) const {
    return title_;
}

void Meeting::setTitle(string title) {
    title_ = title;
}
