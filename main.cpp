#include <QApplication>
#include "mainwindow.h"
#include <QPushButton>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QObject::connect(&a, SIGNAL(aboutToQuit()), &w, SLOT(aboutToQuit()));
    w.show();
    a.setQuitOnLastWindowClosed(false);
    return a.exec();
}
