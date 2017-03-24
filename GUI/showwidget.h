#pragma once

#include "framepainter.h"
#include "imagepainter.h"

#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QOpenGLFunctions>


class QTimer;
class TransferSocketThread;

class ShowWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    ShowWidget(QString title, unsigned int port = 0, QWidget *parent = 0);
    ~ShowWidget();

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    QString m_strTitle;
    unsigned int m_uPort;
    bool m_bFirstTime;
    float m_fAspectRatio;
    TransferSocketThread *m_pTransferSocketThread;

/*******************************************************************************************/
/*openGLœ‡πÿ*/
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int  w, int h) override;

private:
    FramePainter *m_pFramePainter;
    ImagePainter *m_pImagePainter;
    QTimer* m_pTimer;

/********************************************************************************************/

signals:
    void signal_closed(const QString &strWindowTitle);
private slots:
    void slot_update();
};
