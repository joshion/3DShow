#include "mainframe.hpp"
#include "framebuffer.h"
#include "tcpsocket.h"

#include <ConnectProto.pb.h>
#include <KinectDataProto.pb.h>

MainFrame::MainFrame(QObject * parent) 
    : QObject(parent) 
{
    m_pSocket = new TcpSocket;
    m_pSocket->start();
    m_pMainWindow = new MainWindow();
    m_pMainWindow->show();
    //m_pSocket->writeDataToServer();
    connect(m_pMainWindow, &MainWindow::requireConnect, m_pSocket, &TcpSocket::requireConnect);
    connect(m_pMainWindow, &MainWindow::exitConnect, m_pSocket, &TcpSocket::exitConnect);
    connect(m_pMainWindow, &MainWindow::requireDevices, m_pSocket, &TcpSocket::requireDevices);
    connect(m_pMainWindow, &MainWindow::startRequire, m_pSocket, &TcpSocket::startRequire);
    connect(m_pMainWindow, &MainWindow::endConnect, m_pSocket, &TcpSocket::endConnect);
}

MainFrame::~MainFrame()
{
    delete m_pSocket;
    delete m_pMainWindow;
}
