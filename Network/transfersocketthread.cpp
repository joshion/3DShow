#include "transfersocketthread.h"

#include "imagetransfersocket.h"
#include "transferinterface.h"

TransferSocketThread::TransferSocketThread(unsigned int port, QString strIPAdress, 
    TransferInterface *pInterface, QObject *parent)
    : QThread(parent),
    m_strIPAdress(strIPAdress),
    m_uPort(port),
    m_pTransferInterface(pInterface),
    m_pTransferSocket(nullptr)
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
    m_pTransferSocket = new ImageTransferSocket { m_strIPAdress, m_uPort };
    m_pTransferSocket->registerGUIClass(m_pTransferInterface);
    connect(this, &TransferSocketThread::finished, m_pTransferSocket, &ImageTransferSocket::deleteLater);

    exec();
}

