#include "loginwindow.h"
#include <QApplication>
#include <QStyleFactory>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //change theme to fusion
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    LoginWindow w;
    w.show();

    return a.exec();
}
