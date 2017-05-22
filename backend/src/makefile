OBJS = User.o Date.o Meeting.o Storage.o JsonService.o SqliteService.o AgendaUI.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG) -std=c++11
LFLAGS = -Wall $(DEBUG) -std=c++11

Agenda: $(OBJS) Agenda.cpp controller/AgendaService.h
	$(CC) $(LFLAGS) $(OBJS) Agenda.cpp -o Agenda -l sqlite3

User.o: model/User.h model/User.cpp
	$(CC) $(CFLAGS) model/User.cpp
Date.o: model/Date.h model/Date.cpp
	$(CC) $(CFLAGS) model/Date.cpp
Meeting.o: model/Meeting.h model/Meeting.cpp
	$(CC) $(CFLAGS) model/Meeting.cpp

Storage.o: controller/json/Storage.h controller/json/Storage.cpp
	$(CC) $(CFLAGS) controller/json/Storage.cpp
JsonService.o: controller/json/JsonService.h controller/json/JsonService.cpp
	$(CC) $(CFLAGS) controller/json/JsonService.cpp

SqliteService.o: controller/sqlite/SqliteService.h controller/sqlite/SqliteService.cpp
	$(CC) $(CFLAGS) controller/sqlite/SqliteService.cpp

AgendaUI.o: viewer/AgendaUI.h viewer/AgendaUI.cpp
	$(CC) $(CFLAGS) viewer/AgendaUI.cpp

clean:
	rm *.o 
