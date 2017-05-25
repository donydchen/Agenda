#include "ui/qt5/loginwindow.h"
#include <QApplication>
#include <QStyleFactory>
#include <QDir>
#include <QFileInfo>

int main(int argc, char *argv[])
{
    // setup environment
    QString homeDir = QDir::homePath();
    QString envDir = QDir::cleanPath(homeDir + QDir::separator() + ".miniagenda");
    if (!QDir(envDir).exists()) {
        QDir(envDir).mkpath(".");
    }
    QString jsonPath = QDir::cleanPath(envDir + QDir::separator() + "Agenda.json");
    QFileInfo ckFile(jsonPath);
    if (!(ckFile.exists() && ckFile.isFile())) {
        QFile file(jsonPath);
        file.open(QIODevice::ReadWrite);
        file.close();
    }
    QString sqlitePath = QDir::cleanPath(envDir + QDir::separator() + "Agenda.sqlite3");

    QApplication a(argc, argv);
    //change theme to fusion
    QApplication::setStyle(QStyleFactory::create("Fusion"));
    LoginWindow w(jsonPath.toUtf8().constData(), sqlitePath.toUtf8().constData());
    w.show();

    return a.exec();
}
