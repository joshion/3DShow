#include "mainframe.hpp"

#include "ordersocketthread.h"

#include "transfersocketthread.h"

MainFrame::MainFrame(QObject * parent) 
{
    m_pMainWindow = new MainWindow();
    m_pMainWindow->show();

    TransferSocketThread *pTransfer = new TransferSocketThread;

    m_pOrderSocketThread = new OrderSocketThread(m_pMainWindow);
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
    m_pOrderSocketThread->wait();
    delete m_pOrderSocketThread;
    delete m_pMainWindow;
}
