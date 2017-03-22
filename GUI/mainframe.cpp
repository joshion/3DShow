#include "mainframe.hpp"

#include "mainwindow.hpp"
#include "ordersocketthread.h"

#include "skeletonframewidget.h"

MainFrame::MainFrame(QObject * parent)
    :m_pMainWindow(nullptr), m_pOrderSocketThread (nullptr)
{
    m_pMainWindow = new MainWindow();
    m_pMainWindow->show();

    m_pOrderSocketThread = OrderSocketThread::GetInstance("127.0.0.1", 7892, m_pMainWindow);

    SkeletonFrameWidget *w = new SkeletonFrameWidget;
    w->show();

    SkeletonFrameWidget *w2 = new SkeletonFrameWidget;
    w2->show();

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

}

MainFrame::~MainFrame()
{
    if (m_pOrderSocketThread)
    {
        OrderSocketThread::ReleaseInstance();
        m_pOrderSocketThread = nullptr;
    }
  
    if (m_pMainWindow)
    {
        delete m_pMainWindow;
        m_pMainWindow = nullptr;
    }
}