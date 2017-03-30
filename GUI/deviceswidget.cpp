#include "deviceswidget.h"

#include <QMenu>

DevicesWidget::DevicesWidget(QWidget *parent)
    : QListWidget(parent),
    m_pMenu(nullptr),
    m_pStartRequire_Color(nullptr),
    m_pStartRequire_Depth(nullptr),
    m_pStartRequire_Skele(nullptr),
    m_pEndRequireData(nullptr),
    m_pLastClickedItem(nullptr)
{
    QStringList deviceList;
    for (int i = 0; i < 10; ++i)
    {
        deviceList << "aa";
    }
    this->addItems(deviceList);

    m_pStartRequire_Color = new QAction("Show Color", this);
    m_pStartRequire_Depth = new QAction("Show Depth", this);
    m_pStartRequire_Skele = new QAction("Show Skele", this);


    m_pStartRequire_Color_Skele = new QAction("Show Color_Skele", this);
    m_pStartRequire_Depth_Skele = new QAction("Show Depth_Skele", this);
    m_pStartRequire_Color_Depth = new QAction("Show Color_Depth", this);

    m_pStartRequire_Color_Depth_Skele = new QAction("Color_Depth_Skele", this);

    m_pEndRequireData = new QAction("End Require", this);

    connect(m_pStartRequire_Color, &QAction::triggered, this, &DevicesWidget::slot_startRequire_Color);
    connect(m_pStartRequire_Depth, &QAction::triggered, this, &DevicesWidget::slot_startRequire_Depth);
    connect(m_pStartRequire_Skele, &QAction::triggered, this, &DevicesWidget::slot_startRequire_Skele);

    connect(m_pStartRequire_Color_Skele, &QAction::triggered, this, &DevicesWidget::slot_startRequire_Color_Skele);
    connect(m_pStartRequire_Depth_Skele, &QAction::triggered, this, &DevicesWidget::slot_startRequire_Depth_Skele);
    connect(m_pStartRequire_Color_Depth, &QAction::triggered, this, &DevicesWidget::slot_startRequire_Color_Depth);

    connect(m_pStartRequire_Color_Depth_Skele, &QAction::triggered, this, &DevicesWidget::slot_startRequire_Color_Depth_Skele);

    connect(m_pEndRequireData, &QAction::triggered, this, &DevicesWidget::slot_endRequireData);

    QMenu *subMenu = new QMenu("Start Require", this);
    subMenu->addAction(m_pStartRequire_Color);
    subMenu->addAction(m_pStartRequire_Depth);
    subMenu->addAction(m_pStartRequire_Skele);
    subMenu->addAction(m_pStartRequire_Color_Skele);
    subMenu->addAction(m_pStartRequire_Depth_Skele);
    subMenu->addAction(m_pStartRequire_Color_Depth);
    subMenu->addAction(m_pStartRequire_Color_Depth_Skele);

    m_pMenu = new QMenu(this);
    m_pMenu->addMenu(subMenu);
    m_pMenu->addAction(m_pEndRequireData);
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &DevicesWidget::customContextMenuRequested, this, &DevicesWidget::slot_customContextMenuRequested);
}

DevicesWidget::~DevicesWidget()
{
}

void DevicesWidget::slot_startRequire_Color()
{
    if (m_pLastClickedItem)
    {
        QString strWindowTitle = m_pLastClickedItem->text();
        emit signal_createShowWidget(strWindowTitle, Utilities::ShowType::Color);
    }
}

void DevicesWidget::slot_startRequire_Depth()
{
    if (m_pLastClickedItem)
    {
        QString strWindowTitle = m_pLastClickedItem->text();
        emit signal_createShowWidget(strWindowTitle, Utilities::ShowType::Depth);
    }
}

void DevicesWidget::slot_startRequire_Skele()
{
    if (m_pLastClickedItem)
    {
        QString strWindowTitle = m_pLastClickedItem->text();
        emit signal_createShowWidget(strWindowTitle, Utilities::ShowType::Skele);
    }
}

void DevicesWidget::slot_startRequire_Color_Skele()
{
    if (m_pLastClickedItem)
    {
        QString strWindowTitle = m_pLastClickedItem->text();
        emit signal_createShowWidget(strWindowTitle, Utilities::ShowType::Color_Skele);
    }
}

void DevicesWidget::slot_startRequire_Depth_Skele()
{
    if (m_pLastClickedItem)
    {
        QString strWindowTitle = m_pLastClickedItem->text();
        emit signal_createShowWidget(strWindowTitle, Utilities::ShowType::Depth_Skele);
    }
}

void DevicesWidget::slot_startRequire_Color_Depth()
{
    if (m_pLastClickedItem)
    {
        QString strWindowTitle = m_pLastClickedItem->text();
        emit signal_createShowWidget(strWindowTitle, Utilities::ShowType::Color_Depth);
    }
}

void DevicesWidget::slot_startRequire_Color_Depth_Skele()
{
    if (m_pLastClickedItem)
    {
        QString strWindowTitle = m_pLastClickedItem->text();
        emit signal_createShowWidget(strWindowTitle, Utilities::ShowType::Color_Depth_Skele);
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