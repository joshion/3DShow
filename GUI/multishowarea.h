#pragma once

#include "utilities.h"
#include <QMdiArea>
#include <QMap>

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
    * �����Ӵ��ڵ����ֹ������е��Ӵ��� 
    * �˴�Ҫ�����֤ÿ���������ֲ�ͬ 
    */
    QMap<QString, ShowWidget*> m_Title_Widget;

/***********************************************************************************/
/*GUI�ڲ���ͨ��*/
public slots:
    void slot_showSubWidget(QString strWindowTile, Utilities::ShowType type);

private slots:
    void slot_customContextMenuRequested(QPoint point);
    void slot_removeSubWidget(QString title);

/***********************************************************************************/

};
