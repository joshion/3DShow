#ifndef TRANSFERSOCKETTHREAD_H
#define TRANSFERSOCKETTHREAD_H

#include <QThread>

class TransferSocket;
class TransferInterface;

class TransferSocketThread : public QThread
{
    Q_OBJECT

public:
    TransferSocketThread(QString strIPAdress = "127.0.0.1", unsigned int port = 7892, 
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
/***********************************************************************************/
};

#endif // TRANSFERSOCKETTHREAD_H
