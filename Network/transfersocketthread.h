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
    TransferSocketThread(TransferInterface *pInterface = nullptr, Utilities::ShowType type = Utilities::Color,
        QString strIPAdress = "127.0.0.1", unsigned int port = 7893, QObject *parent = nullptr);
    ~TransferSocketThread();
public:
    TransferSocket* getTransferSocketPtr()
    {
        return m_pTransferSocket;
    }
private:
    TransferInterface *m_pTransferInterface;    // 上层GUI类的虚基类, 传递其到下层C++服务,用于上下层的通信
    Utilities::ShowType m_eShowType;
    QString m_strIPAdress;
    unsigned int m_uPort;

/***********************************************************************************/
/*QThread内创建的对象, 需要deleteLater()删除*/
protected:
    void run() override;
private:
    void createTransferSocket(Utilities::ShowType type);
private:
    TransferSocket *m_pTransferSocket;
/***********************************************************************************/
};
