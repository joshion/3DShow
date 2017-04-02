#include "multishowarea.h"
#include "showwidget.h"
#include "utilities.h"

#include <QMenu>
#include <QAction>
#include <QMdiSubWindow>

MultiShowArea::MultiShowArea(QWidget *parent)
    : QMdiArea(parent)
{
    m_p_Title_Widget = new QMap<QString, QMdiSubWindow*>;

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
    /*
    * �ر�������ʱ,���ܻᷢ��
    * ���ͷ�MultiShowArea���ڴ�,���ͷ�m_Title_Widget�д��ڵ���Ϣ,
    * ��m_Title_Widget�еĴ�������Ҫ�õ�m_Title_Widget
    * ��m_Title_Widget��Ҫ����Ϊָ��,��m_Title_Widget�еĴ���ȫ���رպ�,���ͷ�m_Title_Widget���ڴ�
    */
    if (m_p_Title_Widget)
    {
        for (auto a : *m_p_Title_Widget)
        {
            a->close();
            // delete a;
        }
        (*m_p_Title_Widget).clear();
        delete m_p_Title_Widget;
        m_p_Title_Widget = nullptr;
    }
}

/* �����źŵ�orderSocket, �����������ʼ����Kinect���� */
void MultiShowArea::informServerToStartTransfer(const QString &strWindowTile)
{
    m_ReqStart.set_devicename(strWindowTile.toStdString());

    emit signal_reqStart(m_ReqStart);
}

void MultiShowArea::slot_startTransfer(KinectDataProto::pbRespStart respStart)
{
    QString strWindowTile = QString::fromStdString(respStart.devicename());
    if (m_p_Title_Widget->contains(strWindowTile))
    {
        if (Utilities::PROTO_SUCCESS == respStart.resulttype())
        {
            /*
            * ��MDI���ڵ� �Ӵ���ֻ������ShowWidget*���͵Ĵ���
            * �ʿ���ֱ��ת��
            */
            ShowWidget *p = (ShowWidget*) (*m_p_Title_Widget)[strWindowTile]->widget();
            p->KeepThisWidget();
            p->createTransferSocketThreads();
        }
        else if (Utilities::PROTO_FAILURED == respStart.resulttype())
        {
            (*m_p_Title_Widget)[strWindowTile]->close();
            // ��Ӵ��봦��δ�ܳɹ��򿪴����ŵ�
        }
    }
}

void MultiShowArea::slot_closeSubWidget(QString strWindowTitle)
{
    if (m_p_Title_Widget->contains(strWindowTitle))
    {
        (*m_p_Title_Widget)[strWindowTitle]->close();
    }
}

void MultiShowArea::slot_showSubWidget(QString strWindowTitle, Utilities::ShowType type)
{
    if (m_p_Title_Widget->contains(strWindowTitle))
    {
        (*m_p_Title_Widget)[strWindowTitle]->showMaximized();
    }
    else
    {
        QMdiSubWindow *pSubWindow = new QMdiSubWindow;
        ShowWidget *pShowWidget = new ShowWidget { strWindowTitle, type };
        pSubWindow->setWidget(pShowWidget);
        pSubWindow->setAttribute(Qt::WA_DeleteOnClose);
        this->addSubWindow(pSubWindow, Qt::Widget);
        m_p_Title_Widget->insert(strWindowTitle, pSubWindow);

        /* showWidget�ر�ʱ, ������������Ҳ���Ӧ�Ĺر� */
        connect(pShowWidget, &ShowWidget::signal_closed, pSubWindow, &QMdiSubWindow::close);

        /* 
        * �Ӵ��ڹر�ʱ������, ��map�������Ӧ����Ҳ��Ҫɾ��
        * ����Ҫ�����źŵ�ordersocket֪ͨ������,
        * �ͻ����Ѿ��ر����������׽���
        */
        connect(pSubWindow, &QMdiSubWindow::destroyed, [=] {
            if (m_p_Title_Widget)
            {
                m_p_Title_Widget->remove(strWindowTitle);
                KinectDataProto::pbReqEnd reqEnd;
                reqEnd.set_devicename(strWindowTitle.toStdString());
                reqEnd.set_reason("Client require end transfer!");
                emit signal_endRequire(reqEnd);
            }
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