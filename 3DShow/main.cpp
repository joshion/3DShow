/*
#ifdef _MSC_VER
#define _WIN32_WINNT 0x0501
#endif
*/

#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include "StreamPlayProto.pb.h"

using namespace std;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    StreamPlayProto::pbCliReqChangeOrientation orien;
    orien.set_orientation(10);
    orien.set_screenheight(10);
    orien.set_screenwidth(10);

    orien.orientation();


    MainWindow w;
    w.show();

    return a.exec();
}
