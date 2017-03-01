#include <QtWidgets/QApplication>
#include <mainframe.hpp>
#include "StreamPlayProto.pb.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    StreamPlayProto::pbCliReqChangeOrientation orient;
    orient.set_orientation(10);
    orient.set_screenheight(640);
    orient.set_screenwidth(480);
    MainFrame mainframe;
    return a.exec();
}
