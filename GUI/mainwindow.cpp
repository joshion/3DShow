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
    m_pConfig = Config::GetInstance();
    m_pConfig->setIPAdress("192.168.31.250");
    m_pConfig->setServerPort(7892);
    ui.setupUi(this);
    m_pOrderSocketThread = OrderSocketThread::GetInstance(m_pConfig->IPAdress(), m_pConfig->serverPort(), this);

    /* 客户端向服务端请求连接 */
    connect(ui.m_ReqConnect, &QPushButton::clicked, m_pOrderSocketThread, &OrderSocketThread::signal_requireConnect);
    /*
    * 客户端申请断开连接
    * 断开连接时 通过控制socket通知服务器外
    * 也需要在客户端内做出断开所有 数据传输socket
    */
    connect(ui.m_ExitConnect, &QPushButton::clicked, m_pOrderSocketThread, &OrderSocketThread::signal_exitConnect);
    connect(ui.m_ExitConnect, &QPushButton::clicked, this, [this] {
        disconnectFromServer();
    });

    connect(ui.m_MultiShowArea, &MultiShowArea::signal_reqStart, m_pOrderSocketThread, &OrderSocketThread::signal_reqStart);

    /* 从 底层服务orderSocket 发回到 主窗口 的消息 */
    connect(this, &MainWindow::signal_respConnect, this, &MainWindow::slot_respConnect);

    /* 成功从服务器返回设备列表 */
    connect(this, &MainWindow::signal_respDevices, ui.m_DevicesWidget, &DevicesWidget::slot_setDevices);
    connect(this, &MainWindow::signal_respDevices, this, [this] {
        showInfo("Refresh the devices list successed!");
    });

    /* 服务器端主动断开连接 */
    connect(this, &MainWindow::signal_reqEndConnect, this, [this] {
        disconnectFromServer();
        showInfo("Server has ended all connections!");
    });

    /*
    * 依据服务器返回的 回应 信息
    * 如果服务器允许传输,在通知区显示开始传输信息,通知对应窗口开启建立传输,
    * 否则,在通知区显示失败原因,通知对应窗口关闭
    */
    connect(this, &MainWindow::signal_respStart, ui.m_MultiShowArea, &MultiShowArea::slot_startTransfer);
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
    });

    /* 控制socket已经连接, 无需再次连接*/
    connect(this, &MainWindow::signal_hasBeenConnected, this ,[this] {
        showInfo("Has been connected to server, no need to require connect!");
    });

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
    if (m_pConfig)
    {
        Config::ReleaseInstance();
        m_pConfig = nullptr;
    }
}

void MainWindow::showInfo(const QString & info)
{
    QToolTip::showText(QPoint(x() + 260, y() + 23), info, this, QRect(), 2000);
}

void MainWindow::disconnectFromServer()
{
    /*
    * 断开连接后,
    * 通知设备列表窗口清空设备列表,
    * 并不再允许向服务器端请求设备列表
    */
    disconnect(ui.m_ReqDevices, &QPushButton::clicked, m_pOrderSocketThread, &OrderSocketThread::signal_requireDevices);

    // 添加代码, 让设备列表清空列表, 已经打开的显示窗口全部关闭
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
