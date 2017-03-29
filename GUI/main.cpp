#include "mainwindow.hpp"

#include "utilities.h"
#include "singleapplication.h"

#include "ConnectProto.pb.h"
#include "KinectDataProto.pb.h"
#include <QMetaType>

int main(int argc, char *argv[])
{
    qRegisterMetaType<Utilities::ShowType>("Utilities::ShowType");
    qRegisterMetaType<Utilities::SocketType>("Utilities::SocketType");
    qRegisterMetaType<KinectDataProto::pbReqStart>("KinectDataProto::pbReqStart");

    SingleApplication a(argc, argv);
    if (!a.isRunning())
    {
        MainWindow mainwindow;
        a.w = &mainwindow;
        mainwindow.show();
        return a.exec();
    }
    else
    {
        return 0;
    }
}
