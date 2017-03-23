#include "skeletonframewidget.h"

#include "transfersocketthread.h"

#include <QTimer>

SkeletonFrameWidget::SkeletonFrameWidget(unsigned int port, QString title, QWidget *parent)
    : QOpenGLWidget(parent),
    m_uPort(port),
    m_strTitle(title),
    m_pFramePainter(nullptr),
    m_pImagePainter(nullptr),
    m_pTransferSocketThread(nullptr)
{
    this->setWindowTitle(m_strTitle);
    this->setMinimumSize(320, 240);
}

SkeletonFrameWidget::~SkeletonFrameWidget()
{
    if (m_pTransferSocketThread)
    {
        delete m_pTransferSocketThread;
        m_pTransferSocketThread = nullptr;
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

void SkeletonFrameWidget::initializeGL()
{
    /* 0. 初始化函数，使得函数可以使用 */
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);

    /* 只有在第一次show之后才会调用initializeGL(), 所以画图对象在此处创建 */
    m_pFramePainter = new FramePainter;
    m_pImagePainter = new ImagePainter;
    m_pFramePainter->buildShaderProgram("showwidget.vert", "showwidget.frag");
    m_pImagePainter->buildShaderProgram("skeletonframewidget.vert", "skeletonframewidget.frag");

    m_pTransferSocketThread = new TransferSocketThread;
    m_pTimer = new QTimer(this);
    connect(m_pTimer, &QTimer::timeout, this, &SkeletonFrameWidget::slot_update);
    m_pTimer->start(40);
}

void SkeletonFrameWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    if (m_pFramePainter)
    {
        m_pFramePainter->paint();
    }
    if (m_pImagePainter)
    {
        m_pImagePainter->paint();
    }
}

void SkeletonFrameWidget::resizeGL(int w, int h)
{
    if ((float) width() / height() > 640.0 / 480)
    {
        glViewport(0, 0, height() * 640 / 480, height());
    }
    else
    {
        glViewport(0, 0, width(), width() * 480.0 / 640);
    }
}

void SkeletonFrameWidget::slot_update()
{
    if (m_pImagePainter && m_pTransferSocketThread->matsSize() > 0)
    {
        m_pImagePainter->loadTexture(m_pTransferSocketThread->popMat());
    }
    this->update();
}

