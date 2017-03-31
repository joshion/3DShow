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

    /* �ͻ����������������� */
    connect(ui.m_ReqConnect, &QPushButton::clicked, m_pOrderSocketThread, &OrderSocketThread::signal_requireConnect);
    /*
    * �ͻ�������Ͽ�����
    * �Ͽ�����ʱ ͨ������socket֪ͨ��������
    * Ҳ��Ҫ�ڿͻ����������Ͽ����� ���ݴ���socket
    */
    connect(ui.m_ExitConnect, &QPushButton::clicked, m_pOrderSocketThread, &OrderSocketThread::signal_exitConnect);
    connect(ui.m_ExitConnect, &QPushButton::clicked, this, [this] {
        disconnectFromServer();
    });

    connect(ui.m_MultiShowArea, &MultiShowArea::signal_reqStart, m_pOrderSocketThread, &OrderSocketThread::signal_reqStart);

    /* �� �ײ����orderSocket ���ص� ������ ����Ϣ */
    connect(this, &MainWindow::signal_respConnect, this, &MainWindow::slot_respConnect);

    /* �ɹ��ӷ����������豸�б� */
    connect(this, &MainWindow::signal_respDevices, ui.m_DevicesWidget, &DevicesWidget::slot_setDevices);
    connect(this, &MainWindow::signal_respDevices, this, [this] {
        showInfo("Refresh the devices list successed!");
    });

    /* �������������Ͽ����� */
    connect(this, &MainWindow::signal_reqEndConnect, this, [this] {
        disconnectFromServer();
        showInfo("Server has ended all connections!");
    });

    /*
    * ���ݷ��������ص� ��Ӧ ��Ϣ
    * ���������������,��֪ͨ����ʾ��ʼ������Ϣ,֪ͨ��Ӧ���ڿ�����������,
    * ����,��֪ͨ����ʾʧ��ԭ��,֪ͨ��Ӧ���ڹر�
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

    /* ����socket�Ѿ�����, �����ٴ�����*/
    connect(this, &MainWindow::signal_hasBeenConnected, this ,[this] {
        showInfo("Has been connected to server, no need to require connect!");
    });

    /* �� �豸�б��� ���͵� �ര����ʾ���� ����Ϣ */
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
    * �Ͽ����Ӻ�,
    * ֪ͨ�豸�б�������豸�б�,
    * ������������������������豸�б�
    */
    disconnect(ui.m_ReqDevices, &QPushButton::clicked, m_pOrderSocketThread, &OrderSocketThread::signal_requireDevices);

    // ��Ӵ���, ���豸�б�����б�, �Ѿ��򿪵���ʾ����ȫ���ر�
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
