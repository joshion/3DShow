#include "mainwindow.h"

#include "utilities.h"
#include "singleapplication.h"

#include "ConnectProto.pb.h"
#include "KinectDataProto.pb.h"
#include <QMetaType>

int main(int argc, char *argv[])
{
    qRegisterMetaType<Utilities::ShowType>("Utilities::ShowType");
    qRegisterMetaType<Utilities::SocketType>("Utilities::SocketType");
    qRegisterMetaType<ConnectProto::pbRespConnect>("ConnectProto::pbRespConnect");
    qRegisterMetaType<KinectDataProto::pbReqStart>("KinectDataProto::pbReqStart");
    qRegisterMetaType<KinectDataProto::pbRespStart>("KinectDataProto::pbRespStart");
    qRegisterMetaType<KinectDataProto::pbEndTransfer>("KinectDataProto::pbEndTransfer");
    qRegisterMetaType<KinectDataProto::pbReqEnd>("KinectDataProto::pbReqEnd");

    //SingleApplication a(argc, argv);
    //if (!a.isRunning())
    //{
    //    MainWindow mainwindow;
    //    a.w = &mainwindow;
    //    mainwindow.show();
    //    return a.exec();
    //}
    //else
    //{
    //    return 0;
    //}
    QApplication a(argc, argv);

    MainWindow mainwindow;
    mainwindow.show();
    return a.exec();
}