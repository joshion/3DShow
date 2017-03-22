#include "skeletonframewidget.h"

#include "utilities.h"
#include "transfersocketthread.h"

#include "decodevediostream.h"

#include <QTimer>

SkeletonFrameWidget::SkeletonFrameWidget(QWidget *parent)
    : QOpenGLWidget(parent), m_pFramePainter(nullptr), m_pImagePainter(nullptr), m_pTransferSocketThread(nullptr)
{
    m_pTransferSocketThread = new TransferSocketThread;
    m_pTimer = new QTimer(this);
    connect(m_pTimer, &QTimer::timeout, this, &SkeletonFrameWidget::slot_update);
    m_pTimer->start(50);
    show();
}

SkeletonFrameWidget::~SkeletonFrameWidget()
{
    if (m_pTransferSocketThread)
    {
        delete m_pTransferSocketThread;
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
    /* 0. ��ʼ��������ʹ�ú�������ʹ�� */
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);

    /* ֻ���ڵ�һ��show֮��Ż����initializeGL(), ���Ի�ͼ�����ڴ˴����� */
    m_pFramePainter = new FramePainter;
    m_pImagePainter = new ImagePainter;
    m_pFramePainter->buildShaderProgram("showwidget.vert", "showwidget.frag");
    m_pImagePainter->buildShaderProgram("skeletonframewidget.vert", "skeletonframewidget.frag");
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
        if (m_pTransferSocketThread->matsSize() > 0)
        {
            m_pImagePainter->loadTexture(m_pTransferSocketThread->popMats());
        }
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
    this->update();
}

