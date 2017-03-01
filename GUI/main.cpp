#include <QtWidgets/QApplication>
#include <mainframe.hpp>
#include "StreamPlayProto.pb.h"
#include "widget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainFrame mainframe;
    StreamPlayProto::pbCliReqChangeOrientation ori;
    ori.set_orientation(10);
    ori.set_screenheight(10);
    ori.set_screenwidth(10);
    Widget w;
    w.show();

    return a.exec();
}
