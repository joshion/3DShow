#ifndef SHOWWIDGET_H
#define SHOWWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <qopenglfunctions_4_3_core.h>
#include <QOpenGLShaderProgram>
#include <QString>
#include <QFile>

class ShowWidget : public QOpenGLWidget, protected QOpenGLFunctions_4_3_Core
{
    Q_OBJECT

public:
    ShowWidget(QWidget *parent = 0);
    ~ShowWidget();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int  w ,int h);

private:
    QString readStringFromFile(const QString &fileName);

private:
    QOpenGLShaderProgram program;
    QVector4D *triangleVertices;
    QVector4D *color;
};

#endif // SHOWWIDGET_H
