#include "showwidget.h"

#include "transfersocketthread.h"
#include "imagetransfersocket.h"

#include <QTimer>
#include <QDebug>

ShowWidget::ShowWidget(QString title, Utilities::ShowType type, QWidget *parent)
    : QOpenGLWidget(parent),
    m_strTitle(title),
    m_eShowType(type),
    m_bFirstTime(true),
    m_fAspectRatio(4.0 / 3.0),
    m_pColorPainter(nullptr),
    m_pDepthPainter(nullptr),
    m_pSkelePainter(nullptr),
    m_pTimer(nullptr),
    m_UnCreatedPortsCount(0)
{
    this->setWindowTitle(m_strTitle);
    this->setMinimumSize(320, 240);

    createTransferSocketThreads();

    m_protoReqStart.set_devicename(m_strTitle.toStdString());
    m_protoReqStart.set_colorport(-1);
    m_protoReqStart.set_depthport(-1);
    m_protoReqStart.set_skeleport(-1);

    connect(this, &ShowWidget::signal_getLocalPort, this, &ShowWidget::slot_getSocketLocalPort);
    connect(this, &ShowWidget::signal_connectedToServer, this, &ShowWidget::slot_connectedToServer);

}

ShowWidget::~ShowWidget()
{
    for (auto a : m_Type_Socket)
    {
        delete a;
    }
    m_Type_Socket.clear();

    if (m_pTimer)
    {
        delete m_pTimer;
        m_pTimer = nullptr;
    }

    if (m_pColorPainter)
    {
        delete m_pColorPainter;
        m_pColorPainter = nullptr;
    }
    if (m_pDepthPainter)
    {
        delete m_pDepthPainter;
        m_pDepthPainter = nullptr;
    }
    if (m_pSkelePainter)
    {
        delete m_pSkelePainter;
        m_pSkelePainter = nullptr;
    }
}

void ShowWidget::initializeGL()
{
    /* 0. 初始化函数，使得函数可以使用 */
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);

    /* 只有在第一次show之后才会调用initializeGL(), 所以画图对象在此处创建 */
    m_pColorPainter = new ImagePainter;
    m_pDepthPainter = new ImagePainter;
    m_pSkelePainter = new FramePainter;
    m_pColorPainter->buildShaderProgram("image.vert", "image.frag");
    m_pDepthPainter->buildShaderProgram("image.vert", "image.frag");
    m_pSkelePainter->buildShaderProgram("frame.vert", "frame.frag");
}

void ShowWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    resizeGL(this->width(), this->height());

    if (m_pColorPainter)
    {
        m_pColorPainter->paint();
    }
    if (m_pDepthPainter)
    {
        m_pDepthPainter->paint();
    }
    if (m_pSkelePainter)
    {
        m_pSkelePainter->paint();
    }
}

void ShowWidget::resizeGL(int w, int h)
{
    float fAspectRatio = (float) w / h;
    if (fAspectRatio > m_fAspectRatio)  // 窗口的宽度比图片的宽度大
    {
        int x = (w - h * m_fAspectRatio) / 2;
        glViewport(x, 0, h * m_fAspectRatio, h);
    }
    else if (fAspectRatio < m_fAspectRatio) // 窗口的宽度比图片的宽度小
    {
        int y = (h - w / m_fAspectRatio) / 2;
        glViewport(0, y, w, w / m_fAspectRatio);
    }
    else
    {
        glViewport(0, 0, w, h);
    }
}

void ShowWidget::closeEvent(QCloseEvent * event)
{
    emit signal_closed(m_strTitle);
}

void ShowWidget::createTransferSocketThreads()
{
    if (m_eShowType & Utilities::ShowType::Color)
    {
        TransferSocketThread *p = new TransferSocketThread(this, Utilities::SocketType::Color);
        m_Type_Socket.insert(Utilities::ShowType::Color, p);
        ++m_UnCreatedPortsCount;
    }

    if (m_eShowType & Utilities::ShowType::Depth)
    {
        TransferSocketThread *p = new TransferSocketThread(this, Utilities::SocketType::Depth);
        m_Type_Socket.insert(Utilities::ShowType::Depth, p);
        ++m_UnCreatedPortsCount;
    }

    if (m_eShowType & Utilities::ShowType::Skele)
    {
        TransferSocketThread *p = new TransferSocketThread(this, Utilities::SocketType::Skele);
        m_Type_Socket.insert(Utilities::ShowType::Skele, p);
        ++m_UnCreatedPortsCount;
    }
}

void ShowWidget::updateColor()
{
    if (false == m_Type_Socket.contains(Utilities::ShowType::Color))
    {
        return;
    }

    if (m_Type_Socket[Utilities::ShowType::Color] && m_Type_Socket[Utilities::ShowType::Color]->getTransferSocketPtr())
    {
        ImageTransferSocket* m_pSocket = (ImageTransferSocket*) m_Type_Socket[Utilities::ShowType::Color]->getTransferSocketPtr();
        if (m_bFirstTime)
        {
            if (m_pSocket->matsSize() > 0)
            {
                cv::Mat mat = m_pSocket->popMat();
                m_fAspectRatio = (float) mat.cols / mat.rows;
                m_bFirstTime = false;
            }
        }

        if (m_pColorPainter && m_pSocket->matsSize() > 0)
        {
            m_pColorPainter->loadTexture(m_pSocket->popMat());
        }
    }
}

void ShowWidget::updateDepth()
{
}

void ShowWidget::updateSkele()
{
}

void ShowWidget::slot_getSocketLocalPort(Utilities::SocketType type, unsigned int uPort)
{
    if (Utilities::SocketType::Color == type)
    {
        m_protoReqStart.set_colorport(uPort);
    }
    else if (Utilities::SocketType::Depth == type)
    {
        m_protoReqStart.set_depthport(uPort);
    }
    else if (Utilities::SocketType::Skele == type)
    {
        m_protoReqStart.set_skeleport(uPort);
    }
    --m_UnCreatedPortsCount;

    if (m_UnCreatedPortsCount <= 0)
    {
        emit signal_sendBoundPortsToOrderSocket(m_protoReqStart);
    }
    qDebug() << uPort;
}

void ShowWidget::slot_connectedToServer()
{
    firstTimeShow();
}

void ShowWidget::firstTimeShow()
{
    m_pTimer = new QTimer(this);
    connect(m_pTimer, &QTimer::timeout, this, &ShowWidget::slot_update);
    m_pTimer->start(40);
}

void ShowWidget::slot_update()
{
    if (m_eShowType & Utilities::ShowType::Color)
    {
        updateColor();
    }

    if (m_eShowType & Utilities::ShowType::Depth)
    {

    }

    if (m_eShowType & Utilities::ShowType::Skele)
    {

    }
    this->update();
}

