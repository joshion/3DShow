#ifndef TRANSFERSOCKETTHREAD_H
#define TRANSFERSOCKETTHREAD_H

#include <QThread>

class TransferSocket;
class TransferInterface;

class TransferSocketThread : public QThread
{
    Q_OBJECT

public:
    TransferSocketThread(TransferInterface *pInterface = nullptr, QObject *parent = nullptr);
    ~TransferSocketThread();

protected:
    void run() override;

private:
    TransferSocket *m_pTransferSocket;
    TransferInterface *m_pTransferInterface;    // 上层GUI类的虚基类, 传递其到下层C++服务,用于上下层的通信
};

#endif // TRANSFERSOCKETTHREAD_H
