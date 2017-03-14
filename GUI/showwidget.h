#ifndef SHOWWIDGET_H
#define SHOWWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_3_Core>
#include <QOpenGLShaderProgram>

#include <QTimer>

class ShowWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_3_Core
{
    Q_OBJECT

public:
    ShowWidget(QWidget *parent = 0);
    ~ShowWidget();

protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int  w ,int h) override;

private:
    QOpenGLShaderProgram program;
    QVector4D *triangleVertices;
    QVector4D *color;

    QTimer *m_pTimer;

private slots:
    void slot_update();
};

#endif // SHOWWIDGET_H
