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
    /* ���÷�������ַ����� һ��Ҫ������12λ16������ĸ */
    ui.m_ServerName->setValidator(new QRegExpValidator(QRegExp("[0-9a-fA-F]{4}-[0-9a-fA-F]{4}-[0-9a-fA-F]{4}"), ui.m_ServerName));
    ui.m_ServerName->setCursorPosition(0);

    m_pConfig = Config::GetInstance();

    m_pOrderSocketThread = OrderSocketThread::GetInstance(this);

    /* �ͻ����������������� */
    connect(ui.m_ReqConnect, &QPushButton::clicked, this, &MainWindow::slot_login);
    //connect(ui.m_ReqConnect, &QPushButton::clicked, m_pOrderSocketThread, &OrderSocketThread::signal_requireConnect);
    
    /*
    * �ͻ�������Ͽ�����
    * �Ͽ�����ʱ ͨ�������׽���֪ͨ������
    * �ͻ��˵Ķര����ʾ �ر������Ӵ���,���ͬʱҲ�ر������еĴ����׽���
    */
    connect(ui.m_ExitConnect, &QPushButton::clicked, m_pOrderSocketThread, &OrderSocketThread::signal_exitConnect);
    // connect(ui.m_ExitConnect, &QPushButton::clicked, ui.m_MultiShowArea, &MultiShowArea::closeAllSubWindows);
    connect(ui.m_ExitConnect, &QPushButton::clicked, this, [&] {
        disconnectFromServer();
    });

    connect(ui.m_MultiShowArea, &MultiShowArea::signal_reqStart, m_pOrderSocketThread, &OrderSocketThread::signal_reqStart);

    /* ���񷵻ض� �ͻ����������� ����Ӧ */
    connect(this, &MainWindow::signal_respConnect, this, &MainWindow::slot_respConnect, Qt::QueuedConnection);
    /* ����socket�Ѿ�����, �����ٴ����� */
    connect(this, &MainWindow::signal_hasBeenConnected, this, [this] {
        showInfo("Has been connected to server, no need to require connect!");
    }, Qt::QueuedConnection);

    /* �ɹ��ӷ����������豸�б� */
    connect(this, &MainWindow::signal_respDevices, ui.m_DevicesWidget,
        &DevicesWidget::slot_setDevices, Qt::QueuedConnection);
    connect(this, &MainWindow::signal_respDevices, this, [this] {
        showInfo("Refresh the devices list successed!");
    }, Qt::QueuedConnection);

    /* �����쳣�Ͽ����� */
    connect(this, &MainWindow::signal_EndConnect, this, [this] {
        disconnectFromServer();
        showInfo("Server has ended all connections!");
    }, Qt::QueuedConnection);


    /*
    * ���ݷ��������ص� ��Ӧ ��Ϣ
    * ���������������,��֪ͨ����ʾ��ʼ������Ϣ,֪ͨ��Ӧ���ڿ�����������,
    * ����,��֪ͨ����ʾʧ��ԭ��,֪ͨ��Ӧ���ڹر�
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
    * �������˶Ͽ���ĳ���豸�����ݴ�������
    * �رն�Ӧ���豸����ʾ����
    */
    connect(this, &MainWindow::signal_reqEndTransfer, this, [this](KinectDataProto::pbEndTransfer protoEndTransfer) {
        qDebug() << __FILE__ << __LINE__ << "end transfer";
        protoEndTransfer.PrintDebugString();
        QString strWindowTitle = QString::fromStdString(protoEndTransfer.devicename());
        ui.m_MultiShowArea->slot_closeSubWidget(strWindowTitle);
        showInfo("Server has ended " + strWindowTitle + "'s connections! " 
            + QString::fromStdString(protoEndTransfer.reason()));
    }, Qt::QueuedConnection);


    /* �� �豸�б��� ���͵� �ര����ʾ���� ����Ϣ */
    connect(ui.m_DevicesWidget, &DevicesWidget::signal_createShowWidget,
        ui.m_MultiShowArea, &MultiShowArea::slot_createSubWidget);

    /* �豸�б���item���Ҽ�����¼� �رնര����ʾ�е��Ӵ��� */
    connect(ui.m_DevicesWidget, &DevicesWidget::signal_closeShowWidget,
        ui.m_MultiShowArea, &MultiShowArea::slot_closeSubWidget);

    /* ˫���豸�б���item���¼� ��ʾ�ര����ʾ�е��Ӵ��� */
    connect(ui.m_DevicesWidget, &DevicesWidget::signal_showShowWidget,
        ui.m_MultiShowArea, &MultiShowArea::slot_showSubWidget);

    /* �ر�Kinect��ʾ�Ӵ��ڵ�ͬʱ,�ᷢ�Ϳͻ��˽������󵽷����� */
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
    * �Ͽ����Ӻ�,
    * ֪ͨ�豸�б�������豸�б�,
    * ������������������������豸�б�
    */
    disconnect(ui.m_ReqDevices, &QPushButton::clicked, m_pOrderSocketThread, &OrderSocketThread::signal_requireDevices);

    /*
    * ���豸�б�����б�
    * ����ʾ�Ѿ��򿪵���ʾ����ȫ���ر�
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
        /* ���ݷ��������ص�GUID���������ݴ���˿�,�Ա������������ */
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

        /* �������˻�Ӧ���ӳɹ���, ������������������豸�б� */
        connect(ui.m_ReqDevices, &QPushButton::clicked, m_pOrderSocketThread, &OrderSocketThread::signal_requireDevices);
        showInfo("Connected to server successed!");
    }
}
