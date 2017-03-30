#pragma once

#include "KinectDataProto.pb.h"

#include "config.h"

#include "ui_mainwindow.h"
#include "orderinterface.h"
#include <QWidget>

class OrderSocketThread;

class MainWindow : public QWidget, public OrderInterface
{
    Q_OBJECT

public:
    MainWindow(QWidget * parent = Q_NULLPTR);
    ~MainWindow();
private:
    void disconnectFromServer();
private:
    Ui::MainWindow ui;

private:
    Config *m_pConfig;  // �˶����ǵ���,�����ڴ����д������ͷ�һ��
    OrderSocketThread *m_pOrderSocketThread;

/***********************************************************************************/
/*�ײ�C++�뱾GUI��ͨ�����ýӿ�*/
signals: void signal_respConnect() override;
signals: void signal_respDevices(const QStringList& devicesList) override;
signals: void signal_reqEndConnect() override;

signals: void signal_respStart(KinectDataProto::pbRespStart protoRespStart) override;

signals: void signal_hasBeenConnected() override;

private slots:
    void slot_respConnect();
    void slot_reqEndConnect();
    void slot_hasBeenConnected();
/***********************************************************************************/
private slots:
    void slot_exitConnect();
};
