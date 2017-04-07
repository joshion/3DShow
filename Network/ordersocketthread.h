#pragma once

#include "KinectDataProto.pb.h"
#include "singleton.h"
#include <QThread>

class OrderSocket;
class OrderInterface;

class OrderSocketThread : public QThread, public SingleTon<OrderSocketThread>
{
    Q_OBJECT

private:
    OrderSocketThread(OrderInterface *pInterface = nullptr, QObject *parent = nullptr);
    ~OrderSocketThread();

    friend class SingleTon<OrderSocketThread>;

private:
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
signals: void signal_requireConnect(QString ip, unsigned int port);
signals: void signal_exitConnect();
signals: void signal_requireDevices();
signals: void signal_endRequire(KinectDataProto::pbReqEnd reqEnd);

signals: void signal_reqStart(KinectDataProto::pbReqStart reqStart);
/***********************************************************************************/
};
