#include "mainwindow.h"

#include "ordersocketthread.h"

#include "utilities.h"

#include <QString>
#include <QToolTip>
#include <qdebug.h>

MainWindow::MainWindow(QWidget * parent)
    : QWidget(parent),
    m_pConfig(nullptr),
    m_pOrderSocketThread(nullptr)
{
    ui.setupUi(this);
    /* 设置服务器地址输入框 一定要输入齐12位16进制字母 */
    ui.m_ServerName->setValidator(new QRegExpValidator(QRegExp("[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}"), ui.m_ServerName));
    ui.m_ServerName->setCursorPosition(0);

    m_pConfig = Config::GetInstance();

    m_pOrderSocketThread = OrderSocketThread::GetInstance(this);

    /* 客户端向服务端请求连接 */
    connect(ui.m_ReqConnect, &QPushButton::clicked, this, &MainWindow::slot_login);
    //connect(ui.m_ReqConnect, &QPushButton::clicked, m_pOrderSocketThread, &OrderSocketThread::signal_requireConnect);
    
    /*
    * 客户端申请断开连接
    * 断开连接时 通过控制套接字通知服务器
    * 客户端的多窗口显示 关闭所有子窗口,与此同时也关闭了所有的传输套接字
    */
    connect(ui.m_ExitConnect, &QPushButton::clicked, m_pOrderSocketThread, &OrderSocketThread::signal_exitConnect);
    // connect(ui.m_ExitConnect, &QPushButton::clicked, ui.m_MultiShowArea, &MultiShowArea::closeAllSubWindows);
    connect(ui.m_ExitConnect, &QPushButton::clicked, this, [&] {
        disconnectFromServer();
    });

    connect(ui.m_MultiShowArea, &MultiShowArea::signal_reqStart, m_pOrderSocketThread, &OrderSocketThread::signal_reqStart);

    /* 服务返回对 客户端申请连接 的响应 */
    connect(this, &MainWindow::signal_respConnect, this, &MainWindow::slot_respConnect, Qt::QueuedConnection);
    /* 控制socket已经连接, 无需再次连接 */
    connect(this, &MainWindow::signal_hasBeenConnected, this, [this] {
        showInfo("Has been connected to server, no need to require connect!");
    }, Qt::QueuedConnection);

    /* 成功从服务器返回设备列表 */
    connect(this, &MainWindow::signal_respDevices, ui.m_DevicesWidget,
        &DevicesWidget::slot_setDevices, Qt::QueuedConnection);
    connect(this, &MainWindow::signal_respDevices, this, [this] {
        showInfo("Refresh the devices list successed!");
    }, Qt::QueuedConnection);

    /* 网络异常断开连接 */
    connect(this, &MainWindow::signal_EndConnect, this, [this] {
        disconnectFromServer();
        showInfo("Server has ended all connections!");
    }, Qt::QueuedConnection);


    /*
    * 依据服务器返回的 回应 信息
    * 如果服务器允许传输,在通知区显示开始传输信息,通知对应窗口开启建立传输,
    * 否则,在通知区显示失败原因,通知对应窗口关闭
    */
    connect(this, &MainWindow::signal_respStart, ui.m_MultiShowArea,
        &MultiShowArea::slot_startTransfer, Qt::QueuedConnection);
    connect(this, &MainWindow::signal_respStart, this, [this](KinectDataProto::pbRespStart resp) {
        if (Utilities::PROTO_SUCCESS == resp.resulttype())
        {
            showInfo(QString("Server has started transfer ") + QString::fromStdString(resp.devicename()) + "'s data");
        }
        else
        {
            showInfo(QString("Server can\'t transfer ")
                + QString::fromStdString(resp.devicename()) + "\'s data,"
                + QString::fromStdString(resp.failreason()));
        }
    }, Qt::QueuedConnection);

    /*
    * 服务器端断开了某个设备的数据传输连接
    * 关闭对应该设备的显示窗口
    */
    connect(this, &MainWindow::signal_reqEndTransfer, this, [this](KinectDataProto::pbEndTransfer protoEndTransfer) {
        qDebug() << __FILE__ << __LINE__ << "end transfer";
        protoEndTransfer.PrintDebugString();
        QString strWindowTitle = QString::fromStdString(protoEndTransfer.devicename());
        ui.m_MultiShowArea->slot_closeSubWidget(strWindowTitle);
        showInfo("Server has ended " + strWindowTitle + "'s connections! " 
            + QString::fromStdString(protoEndTransfer.reason()));
    }, Qt::QueuedConnection);


    /* 从 设备列表窗口 发送到 多窗口显示窗口 的消息 */
    connect(ui.m_DevicesWidget, &DevicesWidget::signal_createShowWidget,
        ui.m_MultiShowArea, &MultiShowArea::slot_createSubWidget);

    /* 设备列表中item的右键点击事件 关闭多窗口显示中的子窗口 */
    connect(ui.m_DevicesWidget, &DevicesWidget::signal_closeShowWidget,
        ui.m_MultiShowArea, &MultiShowArea::slot_closeSubWidget);

    /* 双击设备列表中item的事件 显示多窗口显示中的子窗口 */
    connect(ui.m_DevicesWidget, &DevicesWidget::signal_showShowWidget,
        ui.m_MultiShowArea, &MultiShowArea::slot_showSubWidget);

    /* 关闭Kinect显示子窗口的同时,会发送客户端结束请求到服务器 */
    connect(ui.m_MultiShowArea, &MultiShowArea::signal_endRequire,
        m_pOrderSocketThread, &OrderSocketThread::signal_endRequire);
}

MainWindow::~MainWindow()
{
    if (m_pOrderSocketThread)
    {
        OrderSocketThread::ReleaseInstance();
        m_pOrderSocketThread = nullptr;
    }
    if (m_pConfig)
    {
        Config::ReleaseInstance();
        m_pConfig = nullptr;
    }
}

void MainWindow::showInfo(const QString & info)
{
    QToolTip::setFont(QFont("Consolas",14));
    QToolTip::showText(QPoint(x() + 420, y() + 23), info, this, QRect(), 5000);
}

void MainWindow::disconnectFromServer()
{
    /*
    * 断开连接后,
    * 通知设备列表窗口清空设备列表,
    * 并不再允许向服务器端请求设备列表
    */
    disconnect(ui.m_ReqDevices, &QPushButton::clicked, m_pOrderSocketThread, &OrderSocketThread::signal_requireDevices);

    /*
    * 让设备列表清空列表
    * 多显示已经打开的显示窗口全部关闭
    */
    ui.m_DevicesWidget->clear();
    ui.m_MultiShowArea->closeAllSubWindows();
}

void MainWindow::slot_login()
{
    if (m_pOrderSocketThread)
    {
        IP_PORT ip_port = Utilities::getAdressFromString(ui.m_ServerName->text());
        m_pOrderSocketThread->signal_requireConnect(ip_port.first, ip_port.second);
    }
}

void MainWindow::slot_respConnect(ConnectProto::pbRespConnect resp)
{
    qDebug() << "enter" << __FILE__ << __LINE__ << "slot_respConnect";

    if (Utilities::PROTO_FAILURED == resp.resulttype())
    {
        showInfo("Can\'t connected to server! Reason:" + QString::fromStdString(resp.failreason()));
    }
    else if (Utilities::PROTO_SUCCESS == resp.resulttype() && m_pConfig)
    {
        /* 根据服务器返回的GUID和三种数据传输端口,对本程序进行配置 */
        m_pConfig->setGuid(QString::fromStdString(resp.guid()));
        if (resp.colorport() > 0)
        {
            m_pConfig->setColorPort(static_cast<unsigned int>(resp.colorport()));
        }
        else
        {
            m_pConfig->setColorPort(0);
        }

        if (resp.depthport() > 0)
        {
            m_pConfig->setDepthPort(static_cast<unsigned int>(resp.depthport()));
        }
        else
        {
            m_pConfig->setDepthPort(0);
        }

        if (resp.skeleport() > 0)
        {
            m_pConfig->setSkelePort(static_cast<unsigned int>(resp.skeleport()));
        }
        else
        {
            m_pConfig->setSkelePort(0);
        }

        /* 服务器端回应连接成功后, 允许向服务器端请求设备列表 */
        connect(ui.m_ReqDevices, &QPushButton::clicked, m_pOrderSocketThread, &OrderSocketThread::signal_requireDevices);
        showInfo("Connected to server successed!");
    }
}
