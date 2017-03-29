#pragma once

#include "utilities.h"

#include "framepainter.h"
#include "imagepainter.h"
#include "transferinterface.h"

#include "KinectDataProto.pb.h"

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
    ShowWidget(QString title, Utilities::ShowType type = Utilities::ShowType::Color, QWidget *parent = 0);
    ~ShowWidget();

/*******************************************************************************************/
/*openGLœ‡πÿ*/
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int  w, int h) override;

private:
    void resizeViewPort();

private:
    ImagePainter *m_pColorPainter;
    ImagePainter *m_pDepthPainter;
    FramePainter *m_pSkelePainter;
    QTimer* m_pTimer;

/********************************************************************************************/

signals: void signal_connectedToServer() override;

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void createTransferSocketThreads();
    void updateColor();
    void updateDepth();
    void updateSkele();
    void firstTimeShow();

signals: void signal_closed(const QString &strWindowTitle);

private slots :
    void slot_update();
    void slot_connectedToServer();

private:
    QString m_strTitle;
    Utilities::ShowType m_eShowType;
    bool m_bFirstTime;
    float m_fAspectRatio;
    QMap<Utilities::ShowType, TransferSocketThread* > m_Type_Socket;

    KinectDataProto::pbReqStart m_protoReqStart;
};
