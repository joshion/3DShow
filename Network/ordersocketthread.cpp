#include "ordersocketthread.h"

#include "ordersocket.h"
#include "orderinterface.hpp"

OrderSocketThread::OrderSocketThread(QString adress, unsigned int port,
    OrderInterface *pInterface, QObject *parent)
    : QThread(parent),
    m_strIPAdress(adress),
    m_uPort(port),
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
    m_pOrderSocket = new OrderSocket { m_strIPAdress, m_uPort };
    m_pOrderSocket->registerGUIClass(m_pOrderInterface);
    connect(this, &OrderSocketThread::finished, m_pOrderSocket, &OrderSocket::deleteLater);

    connect(this, &OrderSocketThread::signal_requireConnect,
        m_pOrderSocket, &OrderSocket::slot_requireConnect, Qt::QueuedConnection);
    connect(this, &OrderSocketThread::signal_exitConnect,
        m_pOrderSocket, &OrderSocket::slot_exitConnect, Qt::QueuedConnection);
    connect(this, &OrderSocketThread::signal_requireDevices,
        m_pOrderSocket, &OrderSocket::slot_requireDevices, Qt::QueuedConnection);


    connect(this, &OrderSocketThread::signal_endRequire,
        m_pOrderSocket, &OrderSocket::slot_endRequire, Qt::QueuedConnection);


    exec();
}
