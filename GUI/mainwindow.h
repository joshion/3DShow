#pragma once

#include "ConnectProto.pb.h"
#include "KinectDataProto.pb.h"

#include "config.h"

#include "ui_mainwindow.h"
#include "orderinterface.h"
#include <QWidget>

class OrderSocketThread;
class QString;

class MainWindow : public QWidget, public OrderInterface
{
    Q_OBJECT

public:
    MainWindow(QWidget * parent = Q_NULLPTR);
    ~MainWindow();
private:
    void showInfo(const QString & info);
    void disconnectFromServer();
private:
    Ui::MainWindow ui;

private:
    /*
    * ȫ�����ö���
    * �����������н���һ��,�ҽ��ڴ����д������ͷ�һ��
    * �ڳ���������߳���ʹ��Config::GetInstance() ��ȡ�˶���ָ��
    */
    Config *m_pConfig;
    OrderSocketThread *m_pOrderSocketThread;

/***********************************************************************************/
/*�ײ�C++�뱾GUI��ͨ�����ýӿ�*/
signals: void signal_respConnect(ConnectProto::pbRespConnect resp) override;
signals: void signal_respDevices(const QStringList& devicesList) override;
signals: void signal_reqEndConnect() override;

signals: void signal_respStart(KinectDataProto::pbRespStart protoRespStart) override;

signals: void signal_hasBeenConnected() override;

private slots:
    void slot_respConnect(ConnectProto::pbRespConnect resp);
/***********************************************************************************/

};
