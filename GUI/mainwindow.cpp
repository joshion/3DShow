#include "mainwindow.hpp"
#include <qdebug.h>
#include "deviceswidget.h"

MainWindow::MainWindow(QWidget * parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    /* 从 主窗口 经过 orderSocketThread 发送到 orderSocket 的消息*/
    connect(ui.m_ReqConnect, &QPushButton::clicked, this, &MainWindow::signal_requireConnect);
    connect(ui.m_ExitConnect, &QPushButton::clicked, this, &MainWindow::signal_exitConnect);
    connect(ui.m_ReqDevices, &QPushButton::clicked, this, &MainWindow::signal_requireDevices);

    /* 从 底层服务orderSocket 发回到 主窗口 的消息 */
    connect(this, &MainWindow::signal_respConnect, this, &MainWindow::slot_respConnect);
    connect(this, &MainWindow::signal_respStartRequire, this, &MainWindow::slot_respStartRequire);
    connect(this, &MainWindow::signal_reqEndConnect, this, &MainWindow::slot_reqEndConnect);
    connect(this, &MainWindow::signal_respDevices, ui.m_DevicesWidget, &DevicesWidget::slot_setDevices);

    /* 从 设备列表窗口 发送到 多窗口显示窗口 的消息 */
    connect(ui.m_DevicesWidget, &DevicesWidget::signal_createShowWidget,
        ui.m_MultiShowArea, &MultiShowArea::slot_showSubWidget);

    connect(ui.m_MultiShowArea, &MultiShowArea::signal_sendPortsToOrderSocket,
        this, &MainWindow::signal_sendPortsToOrderSocket);
}

MainWindow::~MainWindow()
{
	
}

void MainWindow::slot_respConnect()
{
    qDebug() << "enter" << __FILE__ << __LINE__ << "slot_respConnect";
}

void MainWindow::slot_respStartRequire()
{
    qDebug() << "enter" << __FILE__ << __LINE__ << "slot_respStartRequire";
}

void MainWindow::slot_reqEndConnect()
{
    qDebug() << "enter" << __FILE__ << __LINE__ << "slot_reqEndConnect";
}
