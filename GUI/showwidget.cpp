#include "showwidget.h"

#include "transfersocketthread.h"
#include "imagetransfersocket.h"

#include <QTimer>
#include <QDebug>

ShowWidget::ShowWidget(QString title, ShowType type, QWidget *parent)
    : QOpenGLWidget(parent),
    m_strTitle(title),
    m_eShowType(type),
    m_bFirstTime(true),
    m_fAspectRatio(4.0 / 3.0),
    m_pFramePainter(nullptr),
    m_pImagePainter(nullptr),
    m_pTimer(nullptr)
{
    this->setWindowTitle(m_strTitle);
    this->setMinimumSize(320, 240);
    createTransferSocketThreads();
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

    if (m_pImagePainter)
    {
        delete m_pImagePainter;
        m_pImagePainter = nullptr;
    }
    if (m_pFramePainter)
    {
        delete m_pFramePainter;
        m_pFramePainter = nullptr;
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
    m_pFramePainter = new FramePainter;
    m_pImagePainter = new ImagePainter;
    m_pFramePainter->buildShaderProgram("frame.vert", "frame.frag");
    m_pImagePainter->buildShaderProgram("image.vert", "image.frag");
}

void ShowWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    resizeGL(this->width(), this->height());

    if (m_pFramePainter)
    {
        m_pFramePainter->paint();
    }
    if (m_pImagePainter)
    {
        m_pImagePainter->paint();
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
    if (m_eShowType & ShowType::Color)
    {
        TransferSocketThread *p = new TransferSocketThread(this, SocketType::Color);
        m_Type_Socket.insert(ShowType::Color, p);
    }

    if (m_eShowType & ShowType::Depth)
    {
        TransferSocketThread *p = new TransferSocketThread(this, SocketType::Depth);
        m_Type_Socket.insert(ShowType::Depth, p);
    }

    if (m_eShowType & ShowType::Skele)
    {
        TransferSocketThread *p = new TransferSocketThread(this, SocketType::Skele);
        m_Type_Socket.insert(ShowType::Skele, p);
    }
}

void ShowWidget::updateColor()
{
    if (false == m_Type_Socket.contains(ShowType::Color))
    {
        return;
    }

    if (m_Type_Socket[ShowType::Color] && m_Type_Socket[ShowType::Color]->getTransferSocketPtr())
    {
        ImageTransferSocket* m_pSocket = (ImageTransferSocket*) m_Type_Socket[ShowType::Color]->getTransferSocketPtr();
        if (m_bFirstTime)
        {
            if (m_pSocket->matsSize() > 0)
            {
                cv::Mat mat = m_pSocket->popMat();
                m_fAspectRatio = (float) mat.cols / mat.rows;
                m_bFirstTime = false;
            }
        }

        if (m_pImagePainter && m_pSocket->matsSize() > 0)
        {
            m_pImagePainter->loadTexture(m_pSocket->popMat());
        }
    }
}

void ShowWidget::updateDepth()
{
}

void ShowWidget::updateSkele()
{
}

void ShowWidget::slot_getSocketLocalPort(unsigned int uPort)
{
    qDebug() << uPort;
}

void ShowWidget::slot_connectedToServer()
{
}

void ShowWidget::slot_firstTimeShow()
{
    m_pTimer = new QTimer(this);
    connect(m_pTimer, &QTimer::timeout, this, &ShowWidget::slot_update);
    m_pTimer->start(40);
}

void ShowWidget::slot_update()
{
    if (m_eShowType & ShowType::Color)
    {
        updateColor();
    }

    if (m_eShowType & ShowType::Depth)
    {

    }

    if (m_eShowType & ShowType::Skele)
    {

    }
    this->update();
}

