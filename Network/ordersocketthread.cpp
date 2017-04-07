#include "ordersocketthread.h"

#include "ordersocket.h"
#include "orderinterface.h"

OrderSocketThread::OrderSocketThread(OrderInterface *pInterface, QObject *parent)
    : QThread(parent),
    m_pOrderInterface(pInterface),
    m_pOrderSocket(nullptr)
{
    this->start();
}

OrderSocketThread::~OrderSocketThread()
{
    this->quit();
    this->wait(1000);
}

void OrderSocketThread::run()
{
    m_pOrderSocket = new OrderSocket { };
    m_pOrderSocket->registerGUIClass(m_pOrderInterface);

    connect(this, &OrderSocketThread::finished,
        m_pOrderSocket, &OrderSocket::disconnectFromHost, Qt::QueuedConnection);
    connect(this, &OrderSocketThread::finished,
        m_pOrderSocket, &OrderSocket::deleteLater, Qt::QueuedConnection);

    connect(this, &OrderSocketThread::signal_requireConnect,
        m_pOrderSocket, &OrderSocket::slot_requireConnect, Qt::QueuedConnection);
    connect(this, &OrderSocketThread::signal_exitConnect,
        m_pOrderSocket, &OrderSocket::slot_exitConnect, Qt::QueuedConnection);
    connect(this, &OrderSocketThread::signal_requireDevices,
        m_pOrderSocket, &OrderSocket::slot_requireDevices, Qt::QueuedConnection);

    connect(this, &OrderSocketThread::signal_reqStart,
        m_pOrderSocket, &OrderSocket::slot_startRequire, Qt::QueuedConnection);

    connect(this, &OrderSocketThread::signal_endRequire,
        m_pOrderSocket, &OrderSocket::slot_endRequire, Qt::QueuedConnection);

    exec();
}