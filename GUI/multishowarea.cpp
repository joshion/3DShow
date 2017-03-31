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

/* 发送信号到orderSocket, 向服务器请求开始传输Kinect数据 */
void MultiShowArea::informServerToStartTransfer(const QString &strWindowTile)
{
    m_ReqStart.set_devicename(strWindowTile.toStdString());

    emit signal_reqStart(m_ReqStart);
}

void MultiShowArea::slot_startTransfer(KinectDataProto::pbRespStart respStart)
{
    QString strWindowTile = QString::fromStdString(respStart.devicename());
    if (m_Title_Widget.contains(strWindowTile))
    {
        if (Utilities::PROTO_SUCCESS == respStart.resulttype())
        {
            m_Title_Widget[strWindowTile]->createTransferSocketThreads();
        }
        else if (Utilities::PROTO_FAILURED == respStart.resulttype())
        {
            m_Title_Widget[strWindowTile]->close();
            // 添加代码处理未能成功打开传输信道
        }
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

        informServerToStartTransfer(strWindowTitle);
    }
}

void MultiShowArea::slot_customContextMenuRequested(QPoint point)
{
    m_pMenu->exec(QCursor::pos());
}