#ifndef SKELETONFRAMEWIDGET_H
#define SKELETONFRAMEWIDGET_H

#include <QWidget>

#include <opencv2\opencv.hpp>
#include <opencv2\highgui.hpp>

#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions_4_3_Core>

class QGLWidget;
class QTimer;
class QImage;

class SkeletonFrameWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_3_Core
{
    Q_OBJECT

public:
    SkeletonFrameWidget(QWidget *parent = 0);
    ~SkeletonFrameWidget();

/*******************************************************************************************/
/*openGL相关*/
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int  w, int h) override;

private:
    void loadTextures();
private:
    QOpenGLShaderProgram RGBprogram;
    GLuint m_Textures[1];
    void buildRGBProgram();
    void paintRGB();

private:
    QOpenGLShaderProgram skeletonProgram;
    void buildSkeletonProgram();
    void paintFrame();


/********************************************************************************************/

/*******************************************************************************************/
/*openCV相关*/
private:
    static QImage mat2QImage(cv::Mat &mat);
    static QImage mat2GLFormat(cv::Mat &mat);
private:
    cv::Mat m_Mat;
    cv::VideoCapture *m_Capture;
    QTimer *m_Timer;
/********************************************************************************************/

public slots:
    void slot_update();
};

#endif // SKELETONFRAMEWIDGET_H
