#include "multishowarea.h"
#include "showwidget.h"
#include "utilities.h"

#include <QMenu>
#include <QAction>
#include <QMdiSubWindow>

QMap<QString, QMdiSubWindow*> MultiShowArea::m_Title_Widget;

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
    for (auto a : m_Title_Widget)
    {
        a->close();
    }
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
            /*
            * 该MDI窗口的 子窗口只包含了ShowWidget*类型的窗口
            * 故可以直接转换
            */
            ShowWidget *p = (ShowWidget*) m_Title_Widget[strWindowTile]->widget();
            p->KeepThisWidget();
            p->createTransferSocketThreads();
        }
        else if (Utilities::PROTO_FAILURED == respStart.resulttype())
        {
            m_Title_Widget[strWindowTile]->close();
            // 添加代码处理未能成功打开传输信道
        }
    }
}

void MultiShowArea::slot_showSubWidget(QString strWindowTitle, Utilities::ShowType type)
{
    if (m_Title_Widget.contains(strWindowTitle))
    {
        m_Title_Widget[strWindowTitle]->showMaximized();
    }
    else
    {
        QMdiSubWindow *pSubWindow = new QMdiSubWindow;
        ShowWidget *pShowWidget = new ShowWidget { strWindowTitle, type };
        pSubWindow->setWidget(pShowWidget);
        pSubWindow->setAttribute(Qt::WA_DeleteOnClose);
        this->addSubWindow(pSubWindow, Qt::Widget);
        m_Title_Widget.insert(strWindowTitle, pSubWindow);

        /* showWidget关闭时, 包含它的容器也相对应的关闭 */
        connect(pShowWidget, &ShowWidget::signal_closed, pSubWindow, &QMdiSubWindow::close);

        /* 子窗口关闭时即销毁, 在map表的所对应的项也需要删除 */
        connect(pSubWindow, &QMdiSubWindow::destroyed, [=] {
            m_Title_Widget.remove(strWindowTitle);
        });


        pShowWidget->showMaximized();
        pShowWidget = nullptr;
        pSubWindow = nullptr;

        informServerToStartTransfer(strWindowTitle);
    }
}

void MultiShowArea::slot_customContextMenuRequested(QPoint point)
{
    m_pMenu->exec(QCursor::pos());
}