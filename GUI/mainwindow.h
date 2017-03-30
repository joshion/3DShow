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
    Config *m_pConfig;  // 此对象是单例,仅且在此类中创建和释放一次
    OrderSocketThread *m_pOrderSocketThread;

/***********************************************************************************/
/*底层C++与本GUI类通信所用接口*/
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
