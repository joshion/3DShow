/*
#ifdef _MSC_VER
#define _WIN32_WINNT 0x0501
#endif
*/

#include "mainframe.hpp"
#include "StreamPlayProto.pb.h"
#include <QtWidgets/QApplication>

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainFrame mainframe;
    return a.exec();
}
