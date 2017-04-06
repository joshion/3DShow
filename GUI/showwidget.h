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
class QMenu;
class TransferSocketThread;

class ShowWidget : public QOpenGLWidget, public TransferInterface, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    ShowWidget(QString title, Utilities::ShowType type = Utilities::ShowType::Color, QWidget *parent = 0);
    ~ShowWidget();

    /*******************************************************************************************/
    /*openGL相关*/
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int  w, int h) override;

private:
    void resizeViewPort();
    void createMenu();
private:
    ImagePainter *m_pColorPainter;
    ImagePainter *m_pDepthPainter;
    FramePainter *m_pSkelePainter;
    QTimer* m_pTimer;
    QMenu *m_pMenu;

    bool m_bShowColor;
    bool m_bShowDepth;
    bool m_bShowSkele;

    QTimer *m_pTimerCloseSelf;

    /********************************************************************************************/

protected:
    void closeEvent(QCloseEvent *event) override;

private:
    void updateColor();
    void updateDepth();
    void updateSkele();
    void firstTimeShow();

signals: void signal_closed(const QString &strWindowTitle);
signals: void signal_connectedToServer() override;

public:
    void KeepThisWidget();  // 从该窗口构造开始5秒钟内,如果与服务器建立了传输连接,则保留此窗口
    void createTransferSocketThreads();
private slots :
    void slot_update();
    void slot_customContextMenuRequested(QPoint point);
    /*
    * 成功连接到服务器的数据传输端口
    * 开始传输数据
    */
    void slot_connectedToServer();

private:
    QString m_strTitle;
    Utilities::ShowType m_eShowType;
    bool m_bFirstTime;
    float m_fAspectRatio;
    QMap<Utilities::ShowType, TransferSocketThread* > m_Type_Socket;
};
