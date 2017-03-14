#include "skeletonframewidget.h"

#include <QImage>

SkeletonFrameWidget::SkeletonFrameWidget(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    m_Capture = new cv::VideoCapture(0);
    m_Timer = new QTimer(this);
    connect(m_Timer, &QTimer::timeout, this, &SkeletonFrameWidget::showCamera);
    m_Timer->start(50);
}

SkeletonFrameWidget::~SkeletonFrameWidget()
{
    m_Timer->stop();
    delete m_Timer;
    if (m_Capture->isOpened())
    {
        m_Capture->release();
    }
    delete m_Capture;
}

void SkeletonFrameWidget::showMat(cv::Mat &mat)
{
    cvtColor(mat, mat, CV_BGR2RGB);
    QImage image((const unsigned char*) mat.data, mat.cols, mat.rows, QImage::Format_RGB888);
    ui.label->setPixmap(QPixmap::fromImage(image));
}

void SkeletonFrameWidget::showCamera()
{
    *m_Capture >> m_Mat;
    showMat(m_Mat);
}
