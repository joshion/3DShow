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
    if (m_Capture->isOpened())
    {
        m_Capture->release();
        delete m_Capture;
    }
}

void SkeletonFrameWidget::showCamera()
{
    *m_Capture >> m_Mat;
    cvtColor(m_Mat, m_Mat, CV_BGR2RGB);
    QImage qimage = QImage((const unsigned char*) m_Mat.data, m_Mat.cols, m_Mat.rows, QImage::Format_RGB888);
    ui.label->setPixmap(QPixmap::fromImage(qimage));
}
