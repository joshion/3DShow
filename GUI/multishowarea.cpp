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

void MultiShowArea::informServerToStartTransfer(const QString &strWindowTile, Utilities::ShowType type)
{
    m_ReqStart.set_devicename(strWindowTile.toStdString());

    if (Utilities::ShowType::Color & type) { m_ReqStart.set_color(1); }
    else { m_ReqStart.set_color(-1); }

    if (Utilities::ShowType::Depth & type) { m_ReqStart.set_depth(1); }
    else { m_ReqStart.set_depth(-1); }

    if (Utilities::ShowType::Skele & type) { m_ReqStart.set_skele(1); }
    else { m_ReqStart.set_skele(-1); }

    emit signal_reqStart(m_ReqStart);
}

void MultiShowArea::slot_removeSubWidget(QString title)
{
    m_Title_Widget.remove(title);
}

void MultiShowArea::slot_showSubWidget(QString strWindowTitle, Utilities::ShowType type)
{
    if (m_Title_Widget.contains(strWindowTitle))
    {
        m_Title_Widget[strWindowTitle]->showMaximized();
    }
    else
    {
        ShowWidget *p = new ShowWidget { strWindowTitle, type };
        p->setAttribute(Qt::WA_DeleteOnClose);
        this->addSubWindow(p, Qt::Widget);
        p->showMaximized();
        connect(p, &ShowWidget::signal_closed, this, &MultiShowArea::slot_removeSubWidget);

        m_Title_Widget.insert(strWindowTitle, p);
        p = nullptr;

        /* 发送信号到orderSocket, 向服务器请求开始传输数据 */
        informServerToStartTransfer(strWindowTitle, type);
    }
}

void MultiShowArea::slot_customContextMenuRequested(QPoint point)
{
    m_pMenu->exec(QCursor::pos());
}
