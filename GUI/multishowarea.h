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
    * 根据子窗口的名字管理所有的子窗口 
    * 此处要求程序保证每个窗口名字不同 
    */
    QMap<QString, ShowWidget*> m_Title_Widget;

/***********************************************************************************/
/*GUI内部的通信*/
public slots:
    void slot_showSubWidget(QString strWindowTile, Utilities::ShowType type);

private slots:
    void slot_customContextMenuRequested(QPoint point);
    void slot_removeSubWidget(QString title);

/***********************************************************************************/

};
