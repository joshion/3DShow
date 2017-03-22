#include "transfersocketthread.h"

#include "transferinterface.h"

TransferSocketThread::TransferSocketThread(unsigned int port, QString strIPAdress,
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

cv::Mat TransferSocketThread::popMats()
{
    if (m_pTransferSocket)
    {
        return m_pTransferSocket->popMats();
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

