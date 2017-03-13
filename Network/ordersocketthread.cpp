#include "ordersocketthread.h"

#include "ordersocket.h"
#include "orderinterface.hpp"

OrderSocketThread::OrderSocketThread(OrderInterface *pInterface, QObject *parent)
    : m_pOrderInterface(pInterface), QThread(parent)
{
    this->start();
}

OrderSocketThread::~OrderSocketThread()
{
}

void OrderSocketThread::run()
{
    m_pOrderSocket = new OrderSocket {"192.168.1.117"};
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
    m_pOrderSocket->registerGUIClass(m_pOrderInterface);
    m_pOrderSocket->start();
    exec();
}
