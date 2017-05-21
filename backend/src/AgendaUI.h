#ifndef AGENDAUI_H
#define AGENDAUI_H

#include "AgendaService.h"
#include "JsonService.h"
#include <iostream>
#include <string>

class AgendaUI {
 public:
  AgendaUI();
  void OperationLoop(void);

 private:
  std::string userName_;
  std::string userPassword_;
  JsonService jsonService_;
  AgendaService *agendaService_ = &jsonService_;

  // task functions
  void startAgenda(void);
  std::string getOperation();
  bool executeOperation(std::string op);
  void userLogIn(void);
  void userRegister(void);
  void setupBackend(void);
  void quitAgenda(void);
  void userLogOut(void);
  void deleteUser(void);
  void listAllUsers(void);
  void createMeeting(void);
  void listAllMeetings(void);
  void listAllSponsorMeetings(void);
  void listAllParticipateMeetings(void);
  void queryMeetingByTitle(void);
  void queryMeetingByTimeInterval(void);
  void deleteMeetingByTitle(void);
  void deleteAllMeetings(void);
  void printMeetings(std::list<Meeting> meetings);

};

#endif
