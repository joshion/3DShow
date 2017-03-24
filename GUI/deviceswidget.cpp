#include "deviceswidget.h"

#include <QMenu>

DevicesWidget::DevicesWidget(QWidget *parent)
    : QListWidget(parent)
{
    QStringList deviceList;
    for (int i = 0; i < 10; ++i)
    {
        deviceList << "aa";
    }
    this->addItems(deviceList);
    m_pStartRequireData = new QAction("Start Require", this);
    m_pEndRequireData = new QAction("End Require", this);
    connect(m_pStartRequireData, &QAction::triggered, this, &DevicesWidget::slot_startRequireData);
    connect(m_pEndRequireData, &QAction::triggered, this, &DevicesWidget::slot_endRequireData);

    m_pMenu = new QMenu(this);
    m_pMenu->addAction(m_pStartRequireData);
    m_pMenu->addAction(m_pEndRequireData);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &DevicesWidget::customContextMenuRequested, this, &DevicesWidget::slot_customContextMenuRequested);
}

DevicesWidget::~DevicesWidget()
{

}

void DevicesWidget::slot_startRequireData()
{
    emit signal_createSkeletonFrameWidget("hello", 7893);
}

void DevicesWidget::slot_endRequireData()
{
}

void DevicesWidget::slot_customContextMenuRequested(QPoint point)
{
    m_pMenu->exec(QCursor::pos());
}

void DevicesWidget::slot_setDevices(const QStringList& devicesList)
{
    this->clear();
    this->addItems(devicesList);
}
