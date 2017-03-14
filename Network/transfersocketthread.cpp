#include "transfersocketthread.h"

#include "transfersocket.h"
#include "transferinterface.h"

TransferSocketThread::TransferSocketThread(TransferInterface *pInterface, QObject *parent)
    :m_pTransferInterface(pInterface), QThread(parent)
{
    this->start();
}

TransferSocketThread::~TransferSocketThread()
{
    this->quit();
    this->wait(1000);
}

void TransferSocketThread::run()
{
    m_pTransferSocket = new TransferSocket {};

    connect(this, &TransferSocketThread::finished, m_pTransferSocket, &TransferSocket::deleteLater);

    m_pTransferSocket->registerGUIClass(m_pTransferInterface);
    exec();
}
