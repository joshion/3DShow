#pragma once
#include <QMdiArea>

class QMenu;

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

/***********************************************************************************/
/*GUI内部的通信*/
public slots:
    void slot_createSkeletonFrameWidget(QString strWindowTile, unsigned int uPort);

private slots:
    void slot_customContextMenuRequested(QPoint point);

/***********************************************************************************/
};
