## IMPLEMENT CODES for SYSU SS TRNG 2014 - Agenda using Qt

A simple **C++** project for practice, it contains most of the object oriented programming features, such as encapsulation, inheritance, polymorphism, etc. Besides, **SQLite3** and **Qt5** are added, which could help you learn basic database operations and desktop application development. Have Fun!!

![icon](mkpkg/others/icon.png)

### Features 

Agenda is a simple app that can help you manage your daily life. It includes the following useful features:

- [x] User register, delete, login and logout
- [x] Storage selection: JSON / SQLite
- [x] List all users, meetings, sponsor meetings and participator meetings
- [x] Create meeting
- [x] Delete meeting by title, all meetings
- [x] Query meeting by title or by time interval
- [ ] Update meeting information
- [ ] Desktop notification

### Install



```sh
sudo apt install sqlite3
sudo apt install libsqlite3-dev
```  

![](backend/pic/3.png)     

![](backend/pic/4.png)      

![](backend/pic/5.png)


### For Developer

This project is developed using Qt5

```sh
export PATH=/opt/Qt5.7.0/5.7/gcc_64/bin:$PATH
qmake Agenda.pro -r -spec linux-g++
make
```

### Update History

* **May 5th, 2016:** First commit.
* **Oct. 1st, 2016:** Add Qt5 front end, the original terminal based version was moved to subfolder `backend/`.
* **May 24th, 2017:** Add sqlite3 back end in order to adopt [Polymorphism](https://en.wikipedia.org/wiki/Polymorphism_(computer_science) and [Inheritance](https://en.wikipedia.org/wiki/Inheritance_(object-oriented_programming)) features to this project; fine tune UI.
