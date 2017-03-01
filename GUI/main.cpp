#include <QtWidgets/QApplication>
#include <mainframe.hpp>
#include "StreamPlayProto.pb.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainFrame mainframe;

    return a.exec();
}
