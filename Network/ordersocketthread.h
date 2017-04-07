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
    OrderInterface *m_pOrderInterface;  // �ϲ�GUI��������,�����䵽�²�C++����,�������²��ͨ��

/***********************************************************************************/
/*QThread�ڴ����Ķ���, ��ҪdeleteLater()ɾ��*/
protected:
    void run() override;
private:
    OrderSocket *m_pOrderSocket;
/***********************************************************************************/

/***********************************************************************************/
/*GUI�෢����Ϣ���ײ�C++���ýӿ�*/
signals: void signal_requireConnect(QString ip, unsigned int port);
signals: void signal_exitConnect();
signals: void signal_requireDevices();
signals: void signal_endRequire(KinectDataProto::pbReqEnd reqEnd);

signals: void signal_reqStart(KinectDataProto::pbReqStart reqStart);
/***********************************************************************************/
};
