#ifndef ORDERSOCKETTHREAD_H
#define ORDERSOCKETTHREAD_H


#include <QThread>

class OrderSocket;
class OrderInterface;

class OrderSocketThread : public QThread
{
    Q_OBJECT

public:
    OrderSocketThread(OrderInterface &rInterface, QObject *parent);
    ~OrderSocketThread();
protected:
    void run() override;
private:
    OrderSocket *m_pOrderSocket;
    OrderInterface &m_rOrderInterface;  //�ϲ�GUI��,�����䵽�²�C++����,�������²��ͨ��

/***********************************************************************************/
/*GUI�෢����Ϣ���ײ�C++���ýӿ�*/
signals: void signal_requireConnect();
signals: void signal_exitConnect();
signals: void signal_requireDevices();
signals: void signal_startRequire(QString deviceName, unsigned int dataType);
signals: void signal_endConnect();

};

#endif // ORDERSOCKETTHREAD_H
