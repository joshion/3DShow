#include "mainwindow.hpp"

#include "ordersocketthread.h"

#include <qdebug.h>

MainWindow::MainWindow(QWidget * parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    m_pOrderSocketThread = OrderSocketThread::GetInstance("127.0.0.1", 7892, this);

    /* 客户端向服务端请求连接 */
    connect(ui.m_ReqConnect, &QPushButton::clicked, m_pOrderSocketThread, &OrderSocketThread::signal_requireConnect);
    /*
    * 客户端申请断开连接
    * 断开连接时 通过控制socket通知服务器外
    * 也需要在客户端内做出断开所有 数据传输socket
    */
    connect(ui.m_ExitConnect, &QPushButton::clicked, m_pOrderSocketThread, &OrderSocketThread::signal_exitConnect);  
    connect(ui.m_ExitConnect, &QPushButton::clicked, this, &MainWindow::slot_exitConnect);


    /* 从 底层服务orderSocket 发回到 主窗口 的消息 */
    connect(this, &MainWindow::signal_respConnect, this, &MainWindow::slot_respConnect);
    connect(this, &MainWindow::signal_respDevices, ui.m_DevicesWidget, &DevicesWidget::slot_setDevices);
    connect(this, &MainWindow::signal_reqEndConnect, this, &MainWindow::slot_reqEndConnect);    // 服务器端主动断开连接


    connect(ui.m_MultiShowArea, &MultiShowArea::signal_sendBoundPortsToOrderSocket,
        m_pOrderSocketThread, &OrderSocketThread::signal_sendBoundPortsToOrderSocket);

    connect(this, &MainWindow::signal_respStartRequire, ui.m_MultiShowArea, &MultiShowArea::signal_respStartRequire);

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

void MainWindow::disconnectFromServer()
{
    /*
    * 断开连接后, 不再可以让服务器端请求设备列表
    * 并通知设备列表窗口清空设备列表
    */
    disconnect(ui.m_ReqDevices, &QPushButton::clicked, m_pOrderSocketThread, &OrderSocketThread::signal_requireDevices);

    // 添加代码, 让设备列表清空列表, 已经打开的显示窗口全部关闭
}

void MainWindow::slot_respConnect()
{
    /* 服务器端回应连接成功后, 可以让服务器端请求设备列表 */
    connect(ui.m_ReqDevices, &QPushButton::clicked, m_pOrderSocketThread, &OrderSocketThread::signal_requireDevices);
    qDebug() << "enter" << __FILE__ << __LINE__ << "slot_respConnect";
}

/* 服务器主动断开连接 触发事件 */
void MainWindow::slot_reqEndConnect()
{
    disconnectFromServer();
    qDebug() << "enter" << __FILE__ << __LINE__ << "slot_reqEndConnect";
}

/* 退出所有连接时 触发的事件 */
void MainWindow::slot_exitConnect()
{
    disconnectFromServer();
}
