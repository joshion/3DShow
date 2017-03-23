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

    TransferInterface *m_pTransferInterface;    // �ϲ�GUI��������, �����䵽�²�C++����,�������²��ͨ��

/***********************************************************************************/
/*QThread�ڴ����Ķ���, ��ҪdeleteLater()ɾ��*/
protected:
    void run() override;
private:
    TransferSocket *m_pTransferSocket;
public:
    cv::Mat popMat();
    int matsSize();
/***********************************************************************************/
};
