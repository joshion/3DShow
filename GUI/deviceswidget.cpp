#include "deviceswidget.h"

#include <QMenu>

DevicesWidget::DevicesWidget(QWidget *parent)
    : QListWidget(parent),
    m_pMenu(nullptr),
    m_pLastClickedItem(nullptr)
{
    QStringList deviceList;
    for (int i = 0; i < 10; ++i)
    {
        deviceList << QString("aa");
    }
    this->addItems(deviceList);

    createMenu();
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &DevicesWidget::customContextMenuRequested, this, &DevicesWidget::slot_customContextMenuRequested);
}

DevicesWidget::~DevicesWidget()
{
}

void DevicesWidget::createMenu()
{
    if (m_pMenu)
    {
        m_pMenu->clear();
        delete m_pMenu;
        m_pMenu = nullptr;
    }

    m_pMenu = new QMenu(this);

    QMenu *subMenu = new QMenu("Start Require", this);
    QAction *p = nullptr;

    p = new QAction("Show Color", this);
    connect(p, &QAction::triggered, [this] {
        informMultiAreaToCreateWidget(Utilities::ShowType::Color);
    });
    subMenu->addAction(p);

    p = new QAction("Show Depth", this);
    connect(p, &QAction::triggered, [this] {
        informMultiAreaToCreateWidget(Utilities::ShowType::Depth);
    });
    subMenu->addAction(p);

    p = new QAction("Show Skele", this);
    connect(p, &QAction::triggered, [this] {
        informMultiAreaToCreateWidget(Utilities::ShowType::Skele);
    });
    subMenu->addAction(p);

    p = new QAction("Show Color_Skele", this);
    connect(p, &QAction::triggered, [this] {
        informMultiAreaToCreateWidget(Utilities::ShowType::Color_Skele);
    });
    subMenu->addAction(p);

    p = new QAction("Show Depth_Skele", this);
    connect(p, &QAction::triggered, [this] {
        informMultiAreaToCreateWidget(Utilities::ShowType::Depth_Skele);
    });
    subMenu->addAction(p);

    p = new QAction("Show Color_Depth", this);
    connect(p, &QAction::triggered, [this] {
        informMultiAreaToCreateWidget(Utilities::ShowType::Color_Depth);
    });
    subMenu->addAction(p);

    p = new QAction("Show Color_Depth_Skele", this);
    connect(p, &QAction::triggered, [this] {
        informMultiAreaToCreateWidget(Utilities::ShowType::Color_Depth_Skele);
    });
    subMenu->addAction(p);

    m_pMenu->addMenu(subMenu);


    p = new QAction("End Require", this);
    connect(p, &QAction::triggered, this, &DevicesWidget::slot_endRequireData);
    m_pMenu->addAction(p);


    subMenu = nullptr;
    p = nullptr;
}

void DevicesWidget::informMultiAreaToCreateWidget(Utilities::ShowType type)
{
    if (m_pLastClickedItem)
    {
        QString strWindowTitle = m_pLastClickedItem->text();
        emit signal_createShowWidget(strWindowTitle, type);
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