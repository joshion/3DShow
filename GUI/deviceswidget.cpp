#include "deviceswidget.h"

#include <QMenu>

DevicesWidget::DevicesWidget(QWidget *parent)
    : QListWidget(parent),
    m_pMenu(nullptr),
    m_pStartRequireRgb(nullptr),
    m_pStartRequireDepth(nullptr),
    m_pStartRequireSkeleton(nullptr),
    m_pEndRequireData(nullptr),
    m_pLastClickedItem(nullptr)
{
    QStringList deviceList;
    for (int i = 0; i < 10; ++i)
    {
        deviceList << "aa";
    }
    this->addItems(deviceList);

    m_pStartRequireRgb = new QAction("Show RGB", this);
    m_pStartRequireDepth = new QAction("Show Depth", this);
    m_pStartRequireSkeleton = new QAction("Show Skeleton", this);
    m_pEndRequireData = new QAction("End Require", this);

    connect(m_pStartRequireRgb, &QAction::triggered, this, &DevicesWidget::slot_startRequireRgb);
    connect(m_pStartRequireDepth, &QAction::triggered, this, &DevicesWidget::slot_startRequireDepth);
    connect(m_pStartRequireSkeleton, &QAction::triggered, this, &DevicesWidget::slot_startRequireSkeleton);
    connect(m_pEndRequireData, &QAction::triggered, this, &DevicesWidget::slot_endRequireData);

    QMenu *subMenu = new QMenu("Start Require", this);
    subMenu->addAction(m_pStartRequireRgb);
    subMenu->addAction(m_pStartRequireDepth);
    subMenu->addAction(m_pStartRequireSkeleton);

    m_pMenu = new QMenu(this);
    m_pMenu->addMenu(subMenu);
    m_pMenu->addAction(m_pEndRequireData);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &DevicesWidget::customContextMenuRequested, this, &DevicesWidget::slot_customContextMenuRequested);
}

DevicesWidget::~DevicesWidget()
{

}

void DevicesWidget::slot_startRequireRgb()
{
    if (m_pLastClickedItem)
    {
        QString strWindowTitle = m_pLastClickedItem->text();
        emit signal_createSkeletonFrameWidget(strWindowTitle + "_RGB", Utilities::Color);
    }
}

void DevicesWidget::slot_startRequireDepth()
{
    if (m_pLastClickedItem)
    {
        QString strWindowTitle = m_pLastClickedItem->text();
        emit signal_createSkeletonFrameWidget(strWindowTitle + "_Depth", Utilities::Depth);
    }
}

void DevicesWidget::slot_startRequireSkeleton()
{
    if (m_pLastClickedItem)
    {
        QString strWindowTitle = m_pLastClickedItem->text();
        emit signal_createSkeletonFrameWidget(strWindowTitle + "_Skeleton", Utilities::Skele);
    }
}

void DevicesWidget::slot_endRequireData()
{
}

void DevicesWidget::slot_customContextMenuRequested(QPoint point)
{
    m_pLastClickedItem = this->itemAt(point);

    if (m_pLastClickedItem)
    {
        m_pMenu->exec(QCursor::pos());
    }
}

void DevicesWidget::slot_setDevices(const QStringList& devicesList)
{
    this->clear();
    this->addItems(devicesList);
}
