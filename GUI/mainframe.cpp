#include "mainframe.hpp"

#include "mainwindow.hpp"
#include "ordersocketthread.h"
#include "transfersocketthread.h"

#include "skeletonframewidget.h"

MainFrame::MainFrame(QObject * parent) 
{
    m_pMainWindow = new MainWindow();
    m_pMainWindow->show();

    m_pTransferSocketThread = new TransferSocketThread;

    m_pOrderSocketThread = new OrderSocketThread(m_pMainWindow);

    connect(m_pMainWindow, &MainWindow::signal_exitProcess,
        this, &MainFrame::slot_exitProcess);

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
        delete m_pOrderSocketThread;
    }

    if (m_pTransferSocketThread)
    {
        delete m_pTransferSocketThread;
    }
        
    if (m_pMainWindow)
    {
        delete m_pMainWindow;
    }
}

void MainFrame::slot_exitProcess()
{
    m_pOrderSocketThread->exit(0);
    m_pTransferSocketThread->exit(0);
    exit(0);
}
