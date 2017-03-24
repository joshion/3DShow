﻿#pragma once

#include "orderinterface.hpp"

#include "ui_mainwindow.h"
#include <QWidget>
#include <QMap>

class MainWindow : public QWidget ,public OrderInterface
{
	Q_OBJECT

public:
	MainWindow(QWidget * parent = Q_NULLPTR);
	~MainWindow();
private:
	Ui::MainWindow ui;

/***********************************************************************************/
/*GUI内部的通信*/

/***********************************************************************************/

/***********************************************************************************/
/*底层C++与本GUI类通信所用接口*/
signals: void signal_respConnect() override;
signals: void signal_respDevices(const QStringList& devicesList) override;
signals: void signal_respStartRequire() override;
signals: void signal_reqEndConnect() override;

public slots:
    void slot_respConnect();
    void slot_respStartRequire();
    void slot_reqEndConnect();
/***********************************************************************************/

/***********************************************************************************/
/*本GUI类发送消息到底层C++所用接口*/
signals: void signal_requireConnect();
signals: void signal_exitConnect();
signals: void signal_requireDevices();

/***********************************************************************************/
};
