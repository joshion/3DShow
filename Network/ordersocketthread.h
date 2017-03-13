#ifndef ORDERSOCKETTHREAD_H
#define ORDERSOCKETTHREAD_H


#include <QThread>

class OrderSocket;
class OrderInterface;

class OrderSocketThread : public QThread
{
    Q_OBJECT

public:
    OrderSocketThread(OrderInterface *pInterface = nullptr, QObject *parent = nullptr);
    ~OrderSocketThread();
protected:
    void run() override;
private:
    OrderSocket *m_pOrderSocket;
    OrderInterface *m_pOrderInterface;  // 上层GUI类的虚基类,传递其到下层C++服务,用于上下层的通信

/***********************************************************************************/
/*GUI类发送消息到底层C++所用接口*/
signals: void signal_requireConnect();
signals: void signal_exitConnect();
signals: void signal_requireDevices();
signals: void signal_startRequire(QString deviceName, unsigned int dataType);
signals: void signal_endConnect();

};

#endif // ORDERSOCKETTHREAD_H
