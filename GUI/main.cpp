#include "mainframe.hpp"

#include "utilities.h"
#include <QMetaType>

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    qRegisterMetaType<Utilities::ShowType>("Utilities::ShowType");
    qRegisterMetaType<Utilities::SocketType>("Utilities::SocketType");
    QApplication a(argc, argv);
    MainFrame mainframe;

    return a.exec();
}
