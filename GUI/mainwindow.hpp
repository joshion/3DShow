#pragma once

#include "KinectDataProto.pb.h"

#include "ui_mainwindow.h"
#include "orderinterface.hpp"
#include <QWidget>

class OrderSocketThread;

class MainWindow : public QWidget ,public OrderInterface
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
    OrderSocketThread *m_pOrderSocketThread;

/***********************************************************************************/
/*底层C++与本GUI类通信所用接口*/
signals: void signal_respConnect() override;
signals: void signal_respDevices(const QStringList& devicesList) override;
signals: void signal_reqEndConnect() override;

signals: void signal_respStartRequire(KinectDataProto::pbRespStart protoRespStart) override;

public slots:
    void slot_respConnect();
    void slot_reqEndConnect();
/***********************************************************************************/
private slots:
    void slot_exitConnect();
};
