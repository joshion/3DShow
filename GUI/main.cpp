#include <QtWidgets/QApplication>
#include <mainframe.hpp>
#include <ConnectProto.pb.h>
#include <KinectDataProto.pb.h>
#include <network.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainFrame mainframe;
    ConnectProto::pbRespConnect p;
    p.set_resulttype(10);
    p.set_failreason("hellworld");

    Network *pp = new Network;

    return a.exec();
}
