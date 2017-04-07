#include "ordersocket.h"
#include "orderinterface.h"
#include "orderframebuffer.h"

#include "ConnectProto.pb.h"

#include <QTcpSocket>
#include <QString>
#include <QStringList>

#include <QTimer>
#include <QHostInfo>

OrderSocket::OrderSocket(QString adress, unsigned int port, QObject *parent)
    : QTcpSocket(parent),
    m_strIPAdress(adress),
    m_uPort(port),
    m_bConnected(false),
    m_pKeepAliveTimer(nullptr),
    m_pRespKeepAliveTimer(nullptr),
    m_pGUI(nullptr),
    m_pSendFrameBuffer(nullptr),
    m_bNotHasHead(true),
    m_pReceiveFrameBuffer(nullptr),
    m_Sequence_Main(0),
    m_Sequence_RequireConnect(0),
    m_Sequence_RequireDevices(0),
    m_Sequence_StartRequire(0)
{
    m_pSendFrameBuffer = new OrderFrameBuffer {};
    m_pReceiveFrameBuffer = new OrderFrameBuffer {};
    m_receiveBuffer.clear();

    // QString strHostName = QHostInfo::localHostName();

    setSocketOption(QAbstractSocket::KeepAliveOption, 1);
    setSocketOption(QAbstractSocket::LowDelayOption, 1);
    connect(this, &OrderSocket::connected, this, &OrderSocket::slot_setConnected, Qt::QueuedConnection);
    connect(this, &OrderSocket::disconnected, this, &OrderSocket::slot_setDisConnected, Qt::QueuedConnection);
    connect(this, &OrderSocket::readyRead, this, &OrderSocket::slot_readDataFromServer, Qt::QueuedConnection);
}

OrderSocket::~OrderSocket()
{
    this->close();

    if (nullptr == m_pKeepAliveTimer)
    {
        m_pKeepAliveTimer->stop();
        delete m_pKeepAliveTimer;
        m_pKeepAliveTimer = nullptr;
    }

    if (nullptr == m_pRespKeepAliveTimer)
    {
        m_pRespKeepAliveTimer->stop();
        delete m_pRespKeepAliveTimer;
        m_pRespKeepAliveTimer = nullptr;
    }

    if (m_pSendFrameBuffer)
    {
        delete m_pSendFrameBuffer;
        m_pSendFrameBuffer = nullptr;
    }
    if (m_pReceiveFrameBuffer)
    {
        delete m_pReceiveFrameBuffer;
        m_pReceiveFrameBuffer = nullptr;
    }
}

void OrderSocket::slot_setConnected()
{
    m_bConnected = true;

    /*
    * ��������������Ӻ�,
    * ����������֤��Ϣ(����������)
    */
    m_Sequence_RequireConnect = m_Sequence_Main;
    m_pSendFrameBuffer->setSequence(m_Sequence_Main++);
    m_pSendFrameBuffer->setCmdType(OrderFrameBuffer::TYPE_CONNECT_PROTOCOL);
    m_pSendFrameBuffer->setCmdNum(OrderFrameBuffer::NUM_REQUIRE_CONNECT);
    ConnectProto::pbReqConnect reqConnect;
    reqConnect.set_hostname(QHostInfo::localHostName().toStdString());
    m_pSendFrameBuffer->setData(reqConnect);
    writeBufferToServer();

    if (nullptr == m_pKeepAliveTimer)
    {
        m_pKeepAliveTimer = new QTimer(this);
        connect(m_pKeepAliveTimer, &QTimer::timeout, this, &OrderSocket::slot_keepAlive);
        m_pKeepAliveTimer->start(5000);
    }

    if (nullptr == m_pRespKeepAliveTimer)
    {
        m_pRespKeepAliveTimer = new QTimer(this);
        connect(m_pRespKeepAliveTimer, &QTimer::timeout, this, &OrderSocket::slot_AliveOverTime);
        m_pRespKeepAliveTimer->start(30000);
    }
}

void OrderSocket::slot_setDisConnected()
{
    m_bConnected = false;
    if (m_pGUI)
    {
        m_pGUI->signal_EndConnect();
    }

    if (nullptr == m_pKeepAliveTimer)
    {
        m_pKeepAliveTimer->stop();
        delete m_pKeepAliveTimer;
        m_pKeepAliveTimer = nullptr;
    }

    if (nullptr == m_pRespKeepAliveTimer)
    {
        m_pRespKeepAliveTimer->stop();
        delete m_pRespKeepAliveTimer;
        m_pRespKeepAliveTimer = nullptr;
    }
}

bool OrderSocket::writeBufferToServer()
{
    if (m_bConnected)
    {
        QByteArray bytes = OrderFrameBuffer::toBytes(*m_pSendFrameBuffer);
        return write(bytes) == bytes.length();
    }
    else
    {
        return false;
    }
}

bool OrderSocket::writeBufferToServer(const OrderFrameBuffer & buffer)
{
    QByteArray bytes = OrderFrameBuffer::toBytes(buffer);
    return write(bytes) == bytes.length();
}

void OrderSocket::slot_requireConnect()
{
    qDebug() << __FILE__ << __LINE__ << "enter require connect";

    if (m_bConnected)
    {
        m_pGUI->signal_hasBeenConnected();
    }
    else
    {
        connectToHost(m_strIPAdress, m_uPort);
    }
}

void OrderSocket::slot_exitConnect()
{
    qDebug() << __FILE__ << __LINE__ << "enter exit connect";

    m_pSendFrameBuffer->setSequence(m_Sequence_Main++);
    m_pSendFrameBuffer->setCmdType(OrderFrameBuffer::TYPE_CONNECT_PROTOCOL);
    m_pSendFrameBuffer->setCmdNum(OrderFrameBuffer::NUM_EXIT_CONNECT);
    m_pSendFrameBuffer->setData(nullptr, 0);
    this->writeBufferToServer();

    /* �������˳�������Ϣ��,�Ͽ��������֮������� */
    disconnectFromHost();
}

void OrderSocket::slot_requireDevices()
{
    qDebug() << __FILE__ << __LINE__ <<"enter require devices";

    m_Sequence_RequireDevices = m_Sequence_Main;
    m_pSendFrameBuffer->setSequence(m_Sequence_Main++);
    m_pSendFrameBuffer->setCmdType(OrderFrameBuffer::TYPE_CONNECT_PROTOCOL);
    m_pSendFrameBuffer->setCmdNum(OrderFrameBuffer::NUM_REQUIRE_DEVICES);
    m_pSendFrameBuffer->setData(nullptr, 0);
    this->writeBufferToServer();
}

void OrderSocket::slot_startRequire(KinectDataProto::pbReqStart protoReqStart)
{
    qDebug() << __FILE__ << __LINE__ << "enter start require";

    m_Sequence_StartRequire = m_Sequence_Main;
    m_pSendFrameBuffer->setSequence(m_Sequence_Main++);
    m_pSendFrameBuffer->setCmdType(OrderFrameBuffer::TYPE_KINECT_PROTOCOL);
    m_pSendFrameBuffer->setCmdNum(OrderFrameBuffer::NUM_START_REQUIRE);
    m_pSendFrameBuffer->setData(protoReqStart);
    this->writeBufferToServer();
}

void OrderSocket::slot_endRequire(KinectDataProto::pbReqEnd reqEnd)
{
    qDebug() << __FILE__ << __LINE__ << "enter end connect";

    m_pSendFrameBuffer->setSequence(m_Sequence_Main++);
    m_pSendFrameBuffer->setCmdType(OrderFrameBuffer::TYPE_KINECT_PROTOCOL);
    m_pSendFrameBuffer->setCmdNum(OrderFrameBuffer::NUM_END_REQUIRE);
    m_pSendFrameBuffer->setData(reqEnd);
    this->writeBufferToServer();
}

void OrderSocket::slot_keepAlive()
{
    m_pSendFrameBuffer->setSequence(m_Sequence_Main++);
    m_pSendFrameBuffer->setCmdType(OrderFrameBuffer::TYPE_KEEP_ALIVE_PROTOCOL);
    m_pSendFrameBuffer->setCmdNum(OrderFrameBuffer::NUM_KEEP_ALIVE);
    m_pSendFrameBuffer->setData(nullptr, 0);
    this->writeBufferToServer();
}

void OrderSocket::slot_AliveOverTime()
{
    qDebug() << __FILE__ << __LINE__;
    /* 30��û���յ��������ı���ping, �Ͽ��������������*/
    disconnectFromHost();
}

void OrderSocket::analysisReceiveByteArrayBuffer()
{
    /*
    �����ڿ���ճ�������ڶ���ָ��,Ҳ���ܴ��ڲ�������ָ��
    ����ѭ������ָ��,ֱ�����ֲ�������ָ��,�ȴ����յ�����ָ���ʱ���ٽ��н���
    */
    while (true)
    {
        bool bIsACompleteFrameBuffer = false;
        {
            /*
            �������������Ļ�������11�����Ѿ���ȡ����ͷ��ʱ�����˲���,�ȴ��´ζ�ȡ�����໺���ٽ����ж�
            */
            if (m_bNotHasHead && m_receiveBuffer.length() >= m_pReceiveFrameBuffer->headLength())
            {
                m_pReceiveFrameBuffer->setHead(m_receiveBuffer);
                m_receiveBuffer.remove(0, m_pReceiveFrameBuffer->headLength());
                m_bNotHasHead = false;
            }
            /*
            ��û�ж�ȡ��ͷ�����߻���ĳ��ȱ�ͷ��ָ����protobuf�ĳ���Сʱ�����˲���,�ȴ��´ζ�ȡ�����໺��ʱ�ٽ����ж�
            */
            if (!m_bNotHasHead && m_receiveBuffer.length() >= m_pReceiveFrameBuffer->bodyLength())
            {
                m_pReceiveFrameBuffer->setData(m_receiveBuffer, m_pReceiveFrameBuffer->bodyLength());
                m_receiveBuffer.remove(0, m_pReceiveFrameBuffer->bodyLength());
                m_bNotHasHead = true;
                bIsACompleteFrameBuffer = true;
            }
        }
        if (bIsACompleteFrameBuffer)
        {
            analysisReceiveFrameBuffer(*m_pReceiveFrameBuffer);
        }
        else //ָ�����,�ȴ���ȡ��������
        {
            break;
        }
    }
}

void OrderSocket::analysisReceiveFrameBuffer(const OrderFrameBuffer & buffer)
{
    unsigned int sequence = buffer.sequence();
    switch (buffer.cmdType())
    {
    case OrderFrameBuffer::TYPE_CONNECT_PROTOCOL:
        switch (buffer.cmdNum())
        {
        case OrderFrameBuffer::NUM_RESP_REQUIRE_CONNECT:
        {
            ConnectProto::pbRespConnect resp;
            resp.ParseFromArray(buffer.data(), buffer.bodyLength());
            if (m_pGUI && sequence == m_Sequence_RequireConnect)
            {
                m_pGUI->signal_respConnect(resp);
            }
        }
        break;
        case OrderFrameBuffer::NUM_RESP_DEVICES:
        {
            ConnectProto::pbRespDevices resp;
            resp.ParseFromArray(buffer.data(), buffer.bodyLength());
            if (m_pGUI && sequence == m_Sequence_RequireDevices)
            {
                /*
                ��protobuf ����ת��Ϊ �豸�б��͵�GUI�߳�
                */
                QStringList devicesList;
                QString strDevices = QString::fromStdString(resp.devicelist());
                QString strSeparator = QString::fromStdString(resp.separator());
                devicesList = strDevices.split(strSeparator);
                devicesList.removeAll("");
                m_pGUI->signal_respDevices(devicesList);
            }
        }
        break;
        default:
            break;
        }
        break;
    case OrderFrameBuffer::TYPE_KINECT_PROTOCOL :
        switch (buffer.cmdNum())
        {
        case OrderFrameBuffer::NUM_RESP_START_REQUIRE:
        {
            KinectDataProto::pbRespStart resp;
            resp.ParseFromArray(buffer.data(), buffer.bodyLength());
            if (m_pGUI && sequence == m_Sequence_StartRequire)
            {
                m_pGUI->signal_respStart(resp);
            }
        }
        break;
        case OrderFrameBuffer::NUM_SERVER_END_TRANSFER:
        {
            qDebug() << __FILE__ << __LINE__ << "enter end transfer";
            KinectDataProto::pbEndTransfer resp;
            resp.ParseFromArray(buffer.data(), buffer.bodyLength());
            if (m_pGUI)
            {
                m_pGUI->signal_reqEndTransfer(resp);
            }
        }
        break;
        default:
            break;
        }
        break;
    case OrderFrameBuffer::TYPE_KEEP_ALIVE_PROTOCOL:
        switch (buffer.cmdNum())
        {
        case OrderFrameBuffer::NUM_RESP_KEEP_ALIVE:
        {
            if (m_pRespKeepAliveTimer)
            {
                m_pRespKeepAliveTimer->start(30000);
            }
        }
        default:
            break;
        }
        break;
    default:
        break;
    }
}

void OrderSocket::slot_readDataFromServer()
{
    m_receiveBuffer.append(this->readAll());
    analysisReceiveByteArrayBuffer();
}