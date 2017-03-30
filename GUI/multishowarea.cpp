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

/* 发送信号到orderSocket, 向服务器请求返回传输数据所有的端口号 */
void MultiShowArea::informServerToReturnGuid(const QString &strWindowTile)
{
    m_ReqStart.set_devicename(strWindowTile.toStdString());

    emit signal_reqStart(m_ReqStart);
}

void MultiShowArea::slot_startTransfer(KinectDataProto::pbRespStart respStart)
{
    QString strWindowTile = QString::fromStdString(respStart.devicename());
    if (m_Title_Widget.contains(strWindowTile))
    {
        m_Title_Widget[strWindowTile]->createTransferSocketThreads(respStart);
    }
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

        informServerToReturnGuid(strWindowTitle);
    }
}

void MultiShowArea::slot_customContextMenuRequested(QPoint point)
{
    m_pMenu->exec(QCursor::pos());
}
