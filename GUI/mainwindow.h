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
    * 全局配置对象
    * 在整个程序中仅有一份,且仅在此类中创建和释放一次
    * 在程序的其他线程中使用Config::GetInstance() 获取此对象指针
    */
    Config *m_pConfig;
    OrderSocketThread *m_pOrderSocketThread;

/***********************************************************************************/
/*底层C++与本GUI类通信所用接口*/
signals: void signal_respConnect(ConnectProto::pbRespConnect resp) override;
signals: void signal_respDevices(const QStringList& devicesList) override;
signals: void signal_reqEndConnect() override;

signals: void signal_respStart(KinectDataProto::pbRespStart protoRespStart) override;

signals: void signal_hasBeenConnected() override;

private slots:
    void slot_respConnect(ConnectProto::pbRespConnect resp);
/***********************************************************************************/

};
