OBJS = User.o Date.o Meeting.o Storage.o JsonService.o SqliteService.o AgendaUI.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG) -std=c++11
LFLAGS = -Wall $(DEBUG) -std=c++11

Agenda: $(OBJS) Agenda.cpp bll/AgendaService.h
	$(CC) $(LFLAGS) $(OBJS) Agenda.cpp -o Agenda -l sqlite3

User.o: entity/User.h entity/User.cpp
	$(CC) $(CFLAGS) entity/User.cpp
Date.o: entity/Date.h entity/Date.cpp
	$(CC) $(CFLAGS) entity/Date.cpp
Meeting.o: entity/Meeting.h entity/Meeting.cpp
	$(CC) $(CFLAGS) entity/Meeting.cpp

Storage.o: dal/json/Storage.h dal/json/Storage.cpp
	$(CC) $(CFLAGS) dal/json/Storage.cpp

JsonService.o: bll/json/JsonService.h bll/json/JsonService.cpp
	$(CC) $(CFLAGS) bll/json/JsonService.cpp

SqliteService.o: bll/sqlite/SqliteService.h bll/sqlite/SqliteService.cpp
	$(CC) $(CFLAGS) bll/sqlite/SqliteService.cpp

AgendaUI.o: ui/terminal/AgendaUI.h ui/terminal/AgendaUI.cpp
	$(CC) $(CFLAGS) ui/terminal/AgendaUI.cpp

clean:
	rm *.o
