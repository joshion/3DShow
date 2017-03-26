#include "multishowarea.h"
#include "showwidget.h"
#include "utilities.h"

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

void MultiShowArea::slot_removeSubWidget(QString title)
{
    m_Title_Widget.remove(title);
}

void MultiShowArea::slot_showSubWidget(QString strWindowTile, Utilities::ShowType type)
{
    if (m_Title_Widget.contains(strWindowTile))
    {
        m_Title_Widget[strWindowTile]->showMaximized();
    }
    else
    {
        ShowWidget *p = new ShowWidget { strWindowTile, type };
        p->setAttribute(Qt::WA_DeleteOnClose);
        this->addSubWindow(p, Qt::Widget);
        p->showMaximized();
        connect(p, &ShowWidget::signal_closed, this, &MultiShowArea::slot_removeSubWidget);

        connect(p, &ShowWidget::signal_sendPortsToOrderSocket, this, &MultiShowArea::signal_sendPortsToOrderSocket);

        m_Title_Widget.insert(strWindowTile, p);
        p = nullptr;
    }
}

void MultiShowArea::slot_customContextMenuRequested(QPoint point)
{
    m_pMenu->exec(QCursor::pos());
}
