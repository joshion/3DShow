#pragma once

#include "orderinterface.hpp"

#include <QWidget>
#include "ui_mainwindow.h"

class MainWindow : public QWidget ,public OrderInterface
{
	Q_OBJECT

public:
	MainWindow(QWidget * parent = Q_NULLPTR);
	~MainWindow();
protected:
    void closeEvent(QCloseEvent *event) override;
private:
	Ui::MainWindow ui;

/***********************************************************************************/
/*底层C++与本GUI类通信所用接口*/
signals: void signal_respConnect() override;
signals: void signal_respDevices() override;
signals: void signal_respStartRequire() override;
signals: void signal_reqEndConnect() override;

public slots:
    void slot_respConnect();
    void slot_respDevices();
    void slot_respStartRequire();
    void slot_reqEndConnect();
/***********************************************************************************/

/***********************************************************************************/
/*本GUI类发送消息到底层C++所用接口*/
signals: void requireConnect();
signals: void exitConnect();
signals: void requireDevices();
signals: void startRequire(std::string deviceName, unsigned int dataType);
signals: void endConnect();

private slots:
    void slot_startConnect();
/***********************************************************************************/
};
