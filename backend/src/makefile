main: Agenda.cpp User Date Meeting Storage AgendaService AgendaUI
	g++ Agenda.cpp User.o Date.o Meeting.o Storage.o AgendaService.o AgendaUI.o -o Agenda -g -std=c++11

User: User.h User.cpp
	g++ User.h User.cpp -c -std=c++11 -g
Date: Date.h Date.cpp
	g++ Date.h Date.cpp -c -std=c++11 -g
Meeting: Meeting.h Meeting.cpp
	g++ Meeting.h Meeting.cpp -c -std=c++11 -g
Storage: Storage.h Storage.cpp
	g++ Storage.h Storage.cpp -c -std=c++11 -g
AgendaService: AgendaService.h AgendaService.cpp
	g++ AgendaService.h AgendaService.cpp -c -std=c++11 -g
AgendaUI: AgendaUI.h AgendaUI.cpp
	g++ AgendaUI.h AgendaUI.cpp -c -std=c++11 -g

clean:
	rm *.o *.gch Agenda