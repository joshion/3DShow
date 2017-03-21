#include "mainframe.hpp"

#include "mainwindow.hpp"
#include "ordersocketthread.h"

MainFrame::MainFrame(QObject * parent)
    :m_pMainWindow(nullptr), m_pOrderSocketThread (nullptr)
{
    m_pMainWindow = new MainWindow();
    m_pOrderSocketThread = OrderSocketThread::GetInstance("127.0.0.1", 7892, m_pMainWindow);

    connect(m_pMainWindow, &MainWindow::signal_requireConnect,
        m_pOrderSocketThread, &OrderSocketThread::signal_requireConnect, Qt::QueuedConnection);
    connect(m_pMainWindow, &MainWindow::signal_exitConnect,
        m_pOrderSocketThread, &OrderSocketThread::signal_exitConnect, Qt::QueuedConnection);
    connect(m_pMainWindow, &MainWindow::signal_requireDevices,
        m_pOrderSocketThread, &OrderSocketThread::signal_requireDevices, Qt::QueuedConnection);
    connect(m_pMainWindow, &MainWindow::signal_startRequire,
        m_pOrderSocketThread, &OrderSocketThread::signal_startRequire, Qt::QueuedConnection);
    connect(m_pMainWindow, &MainWindow::signal_endConnect,
        m_pOrderSocketThread, &OrderSocketThread::signal_endConnect, Qt::QueuedConnection);

    m_pMainWindow->show();
}

MainFrame::~MainFrame()
{
    OrderSocketThread::ReleaseInstance();
  
    if (m_pMainWindow)
    {
        delete m_pMainWindow;
        m_pMainWindow = nullptr;
    }
}