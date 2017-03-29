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
    OrderSocketThread(QString adress = "127.0.0.1", unsigned int port = 7892, 
        OrderInterface *pInterface = nullptr, QObject *parent = nullptr);
    ~OrderSocketThread();

    friend class SingleTon<OrderSocketThread>;

private:
    QString m_strIPAdress;
    unsigned int m_uPort;

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
signals: void signal_requireConnect();
signals: void signal_exitConnect();
signals: void signal_requireDevices();
signals: void signal_endRequire();

signals: void signal_reqStart(KinectDataProto::pbReqStart reqStart);
/***********************************************************************************/
};
