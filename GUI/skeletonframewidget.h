#ifndef SKELETONFRAMEWIDGET_H
#define SKELETONFRAMEWIDGET_H

#include <QWidget>
#include "ui_skeletonframewidget.h"

#include <opencv2\opencv.hpp>
#include <opencv2\highgui.hpp>
#include <QTimer>

class SkeletonFrameWidget : public QWidget
{
    Q_OBJECT

public:
    SkeletonFrameWidget(QWidget *parent = 0);
    ~SkeletonFrameWidget();

private:
    Ui::SkeletonFrameWidget ui;

private:
    cv::Mat m_Mat;
    cv::VideoCapture *m_Capture;
    QTimer *m_Timer;

private:
    void showMat(cv::Mat &mat);

public slots:
    void showCamera();
};

#endif // SKELETONFRAMEWIDGET_H
