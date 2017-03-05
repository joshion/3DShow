#include <mainframe.hpp>
#include <ConnectProto.pb.h>
#include <KinectDataProto.pb.h>
#include <framebuffer.h>

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainFrame mainframe;
    ConnectProto::pbRespConnect p;
    p.set_resulttype(10);
    p.set_failreason("hellworld");


    unsigned char *pChar = new  unsigned char[10] {"hellwor"};
    FrameBuffer buffer = FrameBuffer(10, 'd', 'd', 10, 'v', pChar);
    QByteArray bytes = FrameBuffer::toByte(buffer);
    FrameBuffer buffer2;
    buffer2 = FrameBuffer::fromByte(bytes);
    return a.exec();
}
