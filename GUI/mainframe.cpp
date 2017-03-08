#include "mainframe.hpp"
#include "framebuffer.h"
#include "ordersocket.h"

#include <ConnectProto.pb.h>
#include <KinectDataProto.pb.h>

MainFrame::MainFrame(QObject * parent) 
    : QObject(parent) 
{
    m_pSocket = new OrderSocket;
    m_pSocket->start();
    m_pMainWindow = new MainWindow();
    m_pMainWindow->show();
    m_pSocket->registerGUIClass(m_pMainWindow);
    connect(m_pMainWindow, &MainWindow::requireConnect, m_pSocket, &OrderSocket::requireConnect);
    connect(m_pMainWindow, &MainWindow::exitConnect, m_pSocket, &OrderSocket::exitConnect);
    connect(m_pMainWindow, &MainWindow::requireDevices, m_pSocket, &OrderSocket::requireDevices);
    connect(m_pMainWindow, &MainWindow::startRequire, m_pSocket, &OrderSocket::startRequire);
    connect(m_pMainWindow, &MainWindow::endConnect, m_pSocket, &OrderSocket::endConnect);
}

MainFrame::~MainFrame()
{
    delete m_pSocket;
    delete m_pMainWindow;
}
