#include "mainwindow.hpp"

//#include "deviceswidget.h"
#include "ordersocketthread.h"

#include <qdebug.h>

MainWindow::MainWindow(QWidget * parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    m_pOrderSocketThread = OrderSocketThread::GetInstance("127.0.0.1", 7892, this);

    /* 从 主窗口 经过 orderSocketThread 发送到 orderSocket 的消息*/
    connect(ui.m_ReqConnect, &QPushButton::clicked,
        m_pOrderSocketThread, &OrderSocketThread::signal_requireConnect, Qt::QueuedConnection);
    connect(ui.m_ExitConnect, &QPushButton::clicked,
        m_pOrderSocketThread, &OrderSocketThread::signal_exitConnect, Qt::QueuedConnection);
    connect(ui.m_ReqDevices, &QPushButton::clicked,
        m_pOrderSocketThread, &OrderSocketThread::signal_requireDevices, Qt::QueuedConnection);
    connect(ui.m_MultiShowArea, &MultiShowArea::signal_sendBoundPortsToOrderSocket,
        m_pOrderSocketThread, &OrderSocketThread::signal_sendBoundPortsToOrderSocket, Qt::QueuedConnection);

    /* 从 底层服务orderSocket 发回到 主窗口 的消息 */
    connect(this, &MainWindow::signal_respConnect, this, &MainWindow::slot_respConnect);
    connect(this, &MainWindow::signal_respStartRequire, ui.m_MultiShowArea, &MultiShowArea::signal_respStartRequire);
    connect(this, &MainWindow::signal_reqEndConnect, this, &MainWindow::slot_reqEndConnect);
    connect(this, &MainWindow::signal_respDevices, ui.m_DevicesWidget, &DevicesWidget::slot_setDevices);

    /* 从 设备列表窗口 发送到 多窗口显示窗口 的消息 */
    connect(ui.m_DevicesWidget, &DevicesWidget::signal_createShowWidget,
        ui.m_MultiShowArea, &MultiShowArea::slot_showSubWidget);
}

MainWindow::~MainWindow()
{
    if (m_pOrderSocketThread)
    {
        OrderSocketThread::ReleaseInstance();
        m_pOrderSocketThread = nullptr;
    }
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
