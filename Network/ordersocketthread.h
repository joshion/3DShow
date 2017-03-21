#ifndef ORDERSOCKETTHREAD_H
#define ORDERSOCKETTHREAD_H

#include "singleton.h"

#include <QThread>

class OrderSocket;
class OrderInterface;

class OrderSocketThread : public QThread, public SingleTon<OrderSocketThread>
{
    Q_OBJECT

protected:
    explicit OrderSocketThread(QString adress = "127.0.0.1", unsigned int port = 7892,
        OrderInterface *pInterface = nullptr, QObject *parent = nullptr);
    ~OrderSocketThread();
    OrderSocketThread(const OrderSocketThread & other) = delete;
    OrderSocketThread& operator= (const OrderSocketThread & other) = delete;
    friend class SingleTon<OrderSocketThread>;

private:
    QString m_strIPAdress;
    unsigned int m_uPort;

    OrderInterface *m_pOrderInterface;  // 上层GUI类的虚基类,传递其到下层C++服务,用于上下层的通信

/***********************************************************************************/
/*QThread内创建的对象, 需要deleteLater()删除*/
protected:
    void run() override;
private:
    OrderSocket *m_pOrderSocket;
/***********************************************************************************/

/***********************************************************************************/
/*GUI类发送消息到底层C++所用接口*/
signals: void signal_requireConnect();
signals: void signal_exitConnect();
signals: void signal_requireDevices();
signals: void signal_startRequire(QString deviceName, unsigned int dataType);
signals: void signal_endConnect();
/***********************************************************************************/
};

#endif // ORDERSOCKETTHREAD_H
