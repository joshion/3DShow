#include "ordersocketthread.h"

#include "ordersocket.h"
#include "orderinterface.hpp"

OrderSocketThread::OrderSocketThread(OrderInterface &rInterface, QObject *parent)
    : m_rOrderInterface(rInterface), QThread(parent)
{
    this->start();
}

OrderSocketThread::~OrderSocketThread()
{
}

void OrderSocketThread::run()
{
    m_pOrderSocket = new OrderSocket;
    connect(this, &OrderSocketThread::signal_requireConnect,
        m_pOrderSocket, &OrderSocket::slot_requireConnect, Qt::QueuedConnection);
    connect(this, &OrderSocketThread::signal_exitConnect,
        m_pOrderSocket, &OrderSocket::slot_exitConnect, Qt::QueuedConnection);
    connect(this, &OrderSocketThread::signal_requireDevices,
        m_pOrderSocket, &OrderSocket::slot_requireDevices, Qt::QueuedConnection);
    connect(this, &OrderSocketThread::signal_startRequire,
        m_pOrderSocket, &OrderSocket::slot_startRequire, Qt::QueuedConnection);
    connect(this, &OrderSocketThread::signal_endConnect,
        m_pOrderSocket, &OrderSocket::slot_endConnect, Qt::QueuedConnection);
    m_pOrderSocket->registerGUIClass(&m_rOrderInterface);
    m_pOrderSocket->start();
    exec();
}
