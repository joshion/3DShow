#include "skeletonframewidget.h"

#include "utilities.h"
#include "transfersocketthread.h"

#include <QTimer>

namespace
{
    const int VERTEX_LOCATION = 1;
    static const int COLOR_LOCATION = 2;
    static const int VIEW_LOCATION = 3;
    static const int PROJECTION_LOCATION = 4;
}

SkeletonFrameWidget::SkeletonFrameWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{

    m_pTransferSocketThread = new TransferSocketThread;

    m_pFramePainter = new FramePainter;
    m_pImagePainter = new ImagePainter;

    m_pTimer = new QTimer(this);
    connect(m_pTimer, &QTimer::timeout, this, &SkeletonFrameWidget::slot_update);
    m_pTimer->start(50);
}

SkeletonFrameWidget::~SkeletonFrameWidget()
{
    delete m_pImagePainter;
    delete m_pFramePainter;
}

void SkeletonFrameWidget::initializeGL()
{
    /* 0. 初始化函数，使得函数可以使用 */
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glEnable(GL_BLEND);
    glEnable(GL_LINE_SMOOTH);

    m_pFramePainter->buildShaderProgram("showwidget.vert", "showwidget.frag");
    m_pImagePainter->buildShaderProgram("skeletonframewidget.vert", "skeletonframewidget.frag");
}

void SkeletonFrameWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    m_pImagePainter->paint();
    m_pFramePainter->paint();
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

