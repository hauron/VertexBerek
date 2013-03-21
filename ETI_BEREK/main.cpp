#include <QtGui/QApplication>
#include "mainsystem.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainSystem system;
    return a.exec();
}

