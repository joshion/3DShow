#pragma once

#include "utilities.h"
#include "KinectDataProto.pb.h"

#include <QMap>
#include <QMdiArea>

class QMenu;
class ShowWidget;

class MultiShowArea : public QMdiArea
{
    Q_OBJECT

public:
    MultiShowArea(QWidget *parent = 0);
    ~MultiShowArea();
private:
    QMenu *m_pMenu;
    QAction *m_pCascadeSubWindows;
    QAction *m_pTileSubWindows;
    /*
    * 根据子窗口的名字管理所有的子窗口
    * 此处要求程序保证每个窗口名字不同
    */
    QMap<QString, ShowWidget*> m_Title_Widget;

private:
    KinectDataProto::pbReqStart m_ReqStart;
private:
    void informServerToStartTransfer(const QString &strWindowTile);

    /* 发送到orderSocket的信号 */
signals: void signal_reqStart(KinectDataProto::pbReqStart reqStart);

/***********************************************************************************/
/*GUI内部的通信*/
public slots:
    void slot_showSubWidget(QString strWindowTile, Utilities::ShowType type);
    /*
    * 处理从orderSocket返回的端口信息
    * 通知对应的子窗口创建数据socket
    * 并连接到服务器返回的端口,接收数据
    */
    void slot_startTransfer(KinectDataProto::pbRespStart respStart);

private slots:
    void slot_customContextMenuRequested(QPoint point);
    void slot_removeSubWidget(QString title);
/***********************************************************************************/
};
