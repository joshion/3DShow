#include "transfersocketthread.h"

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
    m_pTransferSocket = new TransferSocket { m_strIPAdress, m_uPort };
    m_pTransferSocket->registerGUIClass(m_pTransferInterface);
    connect(this, &TransferSocketThread::finished, m_pTransferSocket, &TransferSocket::deleteLater);

    exec();
}

cv::Mat TransferSocketThread::popMat()
{
    if (m_pTransferSocket)
    {
        return m_pTransferSocket->popMat();
    }
    else
    {
        return cv::Mat {};
    }
}

int TransferSocketThread::matsSize()
{
    if (m_pTransferSocket)
    {
        return m_pTransferSocket->matsSize();
    }
    else
    {
        return 0;
    }
}

