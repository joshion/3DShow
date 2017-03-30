#pragma once

#include "transfersocket.h"
#include "utilities.h"
#include <opencv2\opencv.hpp>

#include <QThread>

class TransferSocket;
class TransferInterface;

class TransferSocketThread : public QThread
{
    Q_OBJECT

public:
    TransferSocketThread(QString deviceName, Utilities::SocketType type, QString strIPAdress, unsigned int port,
        TransferInterface *pInterface = nullptr, QObject *parent = nullptr);
    ~TransferSocketThread();
public:
    TransferSocket* getTransferSocketPtr()
    {
        return m_pTransferSocket;
    }
private:
    QString m_strDeviceName;
    Utilities::SocketType m_eSocketType;
    QString m_strIPAdress;
    unsigned int m_uPort;
    TransferInterface *m_pTransferInterface;    // 上层GUI类的虚基类, 传递其到下层C++服务,用于上下层的通信

/***********************************************************************************/
/*QThread内创建的对象, 需要deleteLater()删除*/
protected:
    void run() override;
private:
    void createTransferSocket(Utilities::SocketType type);
private:
    TransferSocket *m_pTransferSocket;
    /***********************************************************************************/
};
