#pragma once

#include "utilities.h"
#include "KinectDataProto.pb.h"

#include <QMap>
#include <QMdiArea>

class QMenu;
class QMdiSubWindow;
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
    * �����Ӵ��ڵ����ֹ������е��Ӵ���
    * �˴�Ҫ�����֤ÿ���������ֲ�ͬ
    */
    QMap<QString, QMdiSubWindow*> *m_Title_Widget;

private:
    KinectDataProto::pbReqStart m_ReqStart;
private:
    void informServerToStartTransfer(const QString &strWindowTile);

    /* ���͵�orderSocket���ź� */
signals: void signal_reqStart(KinectDataProto::pbReqStart reqStart);
signals: void signal_endRequire(KinectDataProto::pbReqEnd reqEnd);
/***********************************************************************************/
/*GUI�ڲ���ͨ��*/
public slots:
    void slot_showSubWidget(QString strWindowTile, Utilities::ShowType type);
    /*
    * �����orderSocket���صĶ˿���Ϣ
    * ֪ͨ��Ӧ���Ӵ��ڴ�������socket
    * �����ӵ����������صĶ˿�,��������
    */
    void slot_startTransfer(KinectDataProto::pbRespStart respStart);
    void slot_endRequire(KinectDataProto::pbReqEnd reqEnd);

private slots:
    void slot_customContextMenuRequested(QPoint point);
/***********************************************************************************/
};
