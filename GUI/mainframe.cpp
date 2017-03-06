﻿#include "mainframe.hpp"
#include "framebuffer.h"
#include "tcpsocket.h"

#include <ConnectProto.pb.h>
#include <KinectDataProto.pb.h>

MainFrame::MainFrame(QObject * parent) 
    : QObject(parent) 
{
    m_pSocket = new TcpSocket;
    m_pMainWindow = new MainWindow;
    m_pMainWindow->show();
    bool flag = m_pSocket->writeDataToServer();
}

MainFrame::~MainFrame()
{
    delete m_pSocket;
    delete m_pMainWindow;
}
