#include "ordersocket.h"
#include "orderinterface.h"
#include "orderframebuffer.h"

#include "config.h"

#include "ConnectProto.pb.h"
#include "KinectDataProto.pb.h"

#include <QTcpSocket>
#include <QString>
#include <QStringList>

#include <QTimer>

namespace
{
    const static unsigned int CONNECT_PROTOCOL = 1;

    const static unsigned int REQUIRE_CONNECT = 1;
    const static unsigned int EXIT_CONNECT = 2;
    const static unsigned int REQUIRE_DEVICES = 3;
    const static unsigned int RESP_REQUIRE_CONNECT = 100;
    const static unsigned int RESP_DEVICES = 101;

    const static unsigned int KINECT_PROTOCOL = 2;

    const static unsigned int START_REQUIRE = 1;
    const static unsigned int END_REQUIRE = 2;
    const static unsigned int RESP_START_REQUIRE = 100;
    const static unsigned int SERVER_END_TRANSFER = 101;
}

OrderSocket::OrderSocket(QString adress, unsigned int port, QObject *parent)
    : QTcpSocket(parent),
    m_strIPAdress(adress),
    m_uPort(port),
    m_bConnected(false),
    m_pGUI(nullptr),
    m_pSendFrameBuffer(nullptr),
    m_bNotHasHead(true),
    m_pReceiveFrameBuffer(nullptr)

{
    m_pSendFrameBuffer = new OrderFrameBuffer {};
    m_pReceiveFrameBuffer = new OrderFrameBuffer {};
    m_receiveBuffer.clear();

    setSocketOption(QAbstractSocket::KeepAliveOption, 1);
    setSocketOption(QAbstractSocket::LowDelayOption, 1);
    // connectToHost(m_strIPAdress, m_uPort);
    connect(this, &OrderSocket::connected, this, &OrderSocket::slot_setConnected, Qt::QueuedConnection);
    connect(this, &OrderSocket::disconnected, this, &OrderSocket::slot_setDisConnected, Qt::QueuedConnection);
    connect(this, &OrderSocket::readyRead, this, &OrderSocket::slot_readDataFromServer, Qt::QueuedConnection);
}

OrderSocket::~OrderSocket()
{
    this->close();

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
    * 与服务器建立连接后,
    * 发送连接验证信息
    */
    m_pSendFrameBuffer->setCmdType(CONNECT_PROTOCOL);
    m_pSendFrameBuffer->setCmdNum(REQUIRE_CONNECT);
    m_pSendFrameBuffer->setData(nullptr, 0);
    writeBufferToServer();
}

void OrderSocket::slot_setDisConnected()
{
    m_bConnected = false;
}

bool OrderSocket::writeBufferToServer()
{
    QByteArray bytes = OrderFrameBuffer::toBytes(*m_pSendFrameBuffer);
    
    if (m_bConnected)
    {
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

bool OrderSocket::slot_requireConnect()
{
    qDebug() << "enter require connect";

    if (m_bConnected)
    {
        m_pGUI->signal_hasBeenConnected();
        return false;
    }
    else
    {
        connectToHost(m_strIPAdress, m_uPort);
        return true;
    }
}

bool OrderSocket::slot_exitConnect()
{
    qDebug() << "enter exit connect";

    m_pSendFrameBuffer->setCmdType(CONNECT_PROTOCOL);
    m_pSendFrameBuffer->setCmdNum(EXIT_CONNECT);
    m_pSendFrameBuffer->setData(nullptr, 0);
    bool flag = this->writeBufferToServer();

    /* 发送完退出连接信息后,断开与服务器之间的连接 */
    disconnectFromHost();

    return flag;
}

bool OrderSocket::slot_requireDevices()
{
    qDebug() << "enter require devices";

    m_pSendFrameBuffer->setCmdType(CONNECT_PROTOCOL);
    m_pSendFrameBuffer->setCmdNum(REQUIRE_DEVICES);
    m_pSendFrameBuffer->setData(nullptr, 0);

    return this->writeBufferToServer();
}

bool OrderSocket::slot_endRequire()
{
    qDebug() << "enter end connect";

    m_pSendFrameBuffer->setCmdType(KINECT_PROTOCOL);
    m_pSendFrameBuffer->setCmdNum(END_REQUIRE);
    m_pSendFrameBuffer->setData(nullptr, 0);

    return this->writeBufferToServer();
}

bool OrderSocket::slot_startRequire(KinectDataProto::pbReqStart protoReqStart)
{
    qDebug() << "enter start require";

    m_pSendFrameBuffer->setCmdType(KINECT_PROTOCOL);
    m_pSendFrameBuffer->setCmdNum(START_REQUIRE);
    m_pSendFrameBuffer->setData(protoReqStart);

    return this->writeBufferToServer();
}

void OrderSocket::analysisReceiveByteArrayBuffer()
{
    /*
    缓存内可能粘包而存在多条指令,也可能存在不完整的指令
    所以循环解析指令,直到发现不完整的指令,等待接收到更多指令的时候再进行解析
    */
    while (true)
    {
        bool bIsACompleteFrameBuffer = false;
        {
            /*
            若服务器发来的缓存少于11或者已经读取到了头部时跳过此部分,等待下次读取到更多缓存再进行判断
            */
            if (m_bNotHasHead && m_receiveBuffer.length() >= m_pReceiveFrameBuffer->headLength())
            {
                m_pReceiveFrameBuffer->setHead(m_receiveBuffer);
                m_receiveBuffer.remove(0, m_pReceiveFrameBuffer->headLength());
                m_bNotHasHead = false;
            }
            /*
            若没有读取到头部或者缓存的长度比头部指定的protobuf的长度小时跳过此部分,等待下次读取到更多缓存时再进行判断
            */
            if (!m_bNotHasHead && m_receiveBuffer.length() >= m_pReceiveFrameBuffer->length())
            {
                m_pReceiveFrameBuffer->setData(m_receiveBuffer, m_pReceiveFrameBuffer->length());
                m_receiveBuffer.remove(0, m_pReceiveFrameBuffer->length());
                m_bNotHasHead = true;
                bIsACompleteFrameBuffer = true;
            }
        }
        if (bIsACompleteFrameBuffer)
        {
            analysisReceiveFrameBuffer(*m_pReceiveFrameBuffer);
        }
        else //指令不完整,等待读取更待缓存
        {
            break;
        }
    }
}

void OrderSocket::analysisReceiveFrameBuffer(const OrderFrameBuffer & buffer)
{
    switch (buffer.cmdType())
    {
    case CONNECT_PROTOCOL:
        switch (buffer.cmdNum())
        {
        case RESP_REQUIRE_CONNECT:
        {
            ConnectProto::pbRespConnect resp;
            resp.ParseFromArray(buffer.data(), buffer.length());
            resp.PrintDebugString();

            /* 根据服务器返回的GUID和三种数据传输端口,对本程序进行配置 */
            Config::GetInstance()->setGuid(QString::fromStdString(resp.guid()));
            if (resp.colorport() > 0)
            {
                Config::GetInstance()->setColorPort(static_cast<unsigned int>(resp.colorport()));
            }
            if (resp.depthport() > 0)
            {
                Config::GetInstance()->setDepthPort(static_cast<unsigned int>(resp.depthport()));
            }
            if (resp.skeleport() > 0)
            {
                Config::GetInstance()->setSkelePort(static_cast<unsigned int>(resp.depthport()));
            }

            if (m_pGUI)
            {
                m_pGUI->signal_respConnect();
            }
        }
        break;
        case RESP_DEVICES:
        {
            ConnectProto::pbRespDevices resp;
            resp.ParseFromArray(buffer.data(), buffer.length());
            resp.PrintDebugString();
            if (m_pGUI)
            {
                /*
                将protobuf 数据转换为 设备列表发送到GUI线程
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
    case KINECT_PROTOCOL:
        switch (buffer.cmdNum())
        {
        case RESP_START_REQUIRE:
        {
            KinectDataProto::pbRespStart resp;
            resp.ParseFromArray(buffer.data(), buffer.length());
            resp.PrintDebugString();
            if (m_pGUI)
            {
                m_pGUI->signal_respStart(resp);
            }

        }
        break;
        case SERVER_END_TRANSFER:
        {

        }
        break;
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
