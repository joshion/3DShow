#include "transfersocketthread.h"

#include "transfersocket.h"
#include "transferinterface.h"

TransferSocketThread::TransferSocketThread(QString strIPAdress, unsigned int port,
    TransferInterface *pInterface, QObject *parent)
    : m_strIPAdress(strIPAdress), m_uPort(port), m_pTransferInterface(pInterface),
    QThread(parent), m_pTransferSocket(nullptr)
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
    m_pTransferSocket = new TransferSocket { m_strIPAdress, m_uPort };

    connect(this, &TransferSocketThread::finished, m_pTransferSocket, &TransferSocket::deleteLater);

    m_pTransferSocket->registerGUIClass(m_pTransferInterface);
    exec();
}
