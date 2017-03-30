#include "mainwindow.h"

#include "ordersocketthread.h"

#include <QToolTip>
#include <qdebug.h>

MainWindow::MainWindow(QWidget * parent)
    : QWidget(parent),
    m_pConfig(nullptr),
    m_pOrderSocketThread(nullptr)
{
    m_pConfig = Config::GetInstance();
    m_pConfig->setIPAdress("127.0.0.1");
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
    connect(ui.m_ExitConnect, &QPushButton::clicked, this, &MainWindow::slot_exitConnect);

    connect(ui.m_MultiShowArea, &MultiShowArea::signal_reqStart, m_pOrderSocketThread, &OrderSocketThread::signal_reqStart);

    /* �� �ײ����orderSocket ���ص� ������ ����Ϣ */
    connect(this, &MainWindow::signal_respConnect, this, &MainWindow::slot_respConnect);
    connect(this, &MainWindow::signal_respDevices, ui.m_DevicesWidget, &DevicesWidget::slot_setDevices);
    connect(this, &MainWindow::signal_reqEndConnect, this, &MainWindow::slot_reqEndConnect);    // �������������Ͽ�����
    connect(this, &MainWindow::signal_respStart, ui.m_MultiShowArea, &MultiShowArea::slot_startTransfer);   // ���������ض˿ں�
    connect(this, &MainWindow::signal_hasBeenConnected, this, &MainWindow::slot_hasBeenConnected);  // ����socket�Ѿ�����

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

void MainWindow::slot_respConnect()
{
    /* �������˻�Ӧ���ӳɹ���, ������������������豸�б� */
    connect(ui.m_ReqDevices, &QPushButton::clicked, m_pOrderSocketThread, &OrderSocketThread::signal_requireDevices);

    QToolTip::showText(QPoint(x() + 250, y()), "connected to server successed!", this, QRect(), 2000);
    qDebug() << "enter" << __FILE__ << __LINE__ << "slot_respConnect";
}

/* �����������Ͽ����� �����¼� */
void MainWindow::slot_reqEndConnect()
{
    disconnectFromServer();
    qDebug() << "enter" << __FILE__ << __LINE__ << "slot_reqEndConnect";
}

/* ����socket�Ѿ��ɹ����ӵ��������� �ٵ����������ʱ �ᴥ�����¼� */
void MainWindow::slot_hasBeenConnected()
{
    QToolTip::showText(QPoint(x() + 250, y()),
        "Has been connected to server,\n no need to require connect!", this, QRect(), 2000);
}

/* �˳���������ʱ �������¼� */
void MainWindow::slot_exitConnect()
{
    disconnectFromServer();
    qDebug() << "enter" << __FILE__ << __LINE__ << "slot_exitConnect";
}