#include "multishowarea.h"
#include "skeletonframewidget.h"

#include <QMenu>
#include <QAction>

MultiShowArea::MultiShowArea(QWidget *parent)
    : QMdiArea(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    m_pCascadeSubWindows = new QAction("Cascade SubWindows", this);
    m_pTileSubWindows = new QAction("Tile SubWindows", this);
    connect(m_pCascadeSubWindows, &QAction::triggered, this, &MultiShowArea::cascadeSubWindows);
    connect(m_pTileSubWindows, &QAction::triggered, this, &MultiShowArea::tileSubWindows);

    m_pMenu = new QMenu(this);
    m_pMenu->addAction(m_pCascadeSubWindows);
    m_pMenu->addAction(m_pTileSubWindows);
    connect(this, &MultiShowArea::customContextMenuRequested, this, &MultiShowArea::slot_customContextMenuRequested);
}

MultiShowArea::~MultiShowArea()
{

}

void MultiShowArea::slot_createSkeletonFrameWidget(QString strWindowTile, unsigned int uPort)
{
    SkeletonFrameWidget *p = new SkeletonFrameWidget { 7893 };
    p->setAttribute(Qt::WA_DeleteOnClose);
    this->addSubWindow(p, Qt::Widget);
    p->showMaximized();
}

void MultiShowArea::slot_customContextMenuRequested(QPoint point)
{
    m_pMenu->exec(QCursor::pos());
}
