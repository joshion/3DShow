#pragma once

#include "transfersocket.h"

#include <opencv2\opencv.hpp>

#include <QThread>

class TransferSocket;
class TransferInterface;

class TransferSocketThread : public QThread
{
    Q_OBJECT

public:
    TransferSocketThread(unsigned int port = 7893, QString strIPAdress = "127.0.0.1",
        TransferInterface *pInterface = nullptr, QObject *parent = nullptr);
    ~TransferSocketThread();
private:
    QString m_strIPAdress;
    unsigned int m_uPort;

    TransferInterface *m_pTransferInterface;    // 上层GUI类的虚基类, 传递其到下层C++服务,用于上下层的通信

/***********************************************************************************/
/*QThread内创建的对象, 需要deleteLater()删除*/
protected:
    void run() override;
private:
    TransferSocket *m_pTransferSocket;
public:
    cv::Mat popMat();
    int matsSize();
/***********************************************************************************/
};
