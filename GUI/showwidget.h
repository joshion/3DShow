#pragma once

#include "utilities.h"

#include "framepainter.h"
#include "imagepainter.h"
#include "transferinterface.h"

#include <QMap>
#include <QOpenGLWidget>
#include <QOpenGLContext>
#include <QOpenGLFunctions>


class QTimer;
class TransferSocketThread;

class ShowWidget : public QOpenGLWidget, public TransferInterface , protected QOpenGLFunctions
{
    Q_OBJECT

public:
    ShowWidget(QString title, ShowType type = ShowType::Color, QWidget *parent = 0);
    ~ShowWidget();

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

signals: void signal_getLocalPort(unsigned int uPort) override;
signals: void signal_connectedToServer() override;

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void createTransferSocketThreads();
    void updateColor();
    void updateDepth();
    void updateSkele();
private:
    QString m_strTitle;
    ShowType m_eShowType;
    bool m_bFirstTime;
    float m_fAspectRatio;
    QMap<ShowType, TransferSocketThread* > m_Type_Socket;


signals:
    void signal_closed(const QString &strWindowTitle);
private slots:
    void slot_update();
};
