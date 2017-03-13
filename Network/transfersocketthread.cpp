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

}

void TransferSocketThread::run()
{
    m_pTransferSocket = new TransferSocket {};
    m_pTransferSocket->registerGUIClass(m_pTransferInterface);
    m_pTransferSocket->start();
    exec();
}
