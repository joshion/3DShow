#include "mainframe.hpp"

#include <ConnectProto.pb.h>
#include <KinectDataProto.pb.h>
#include <framebuffer.h>

MainFrame::MainFrame(QObject * parent) 
    : QObject(parent) 
{
    m_pMainWindow = new MainWindow;
    m_pMainWindow->show();

    ConnectProto::pbRespConnect p;
    p.set_resulttype(10);
    p.set_failreason("hellworld");

    FrameBuffer buffer(10, 'd', 'd', 10, 'v', nullptr);
    QByteArray bytes = FrameBuffer::toByte(buffer);
    FrameBuffer buffer2;
    buffer2 = FrameBuffer::fromByte(bytes);
    buffer2.~FrameBuffer();
}

MainFrame::~MainFrame()
{
    delete m_pMainWindow;
}
