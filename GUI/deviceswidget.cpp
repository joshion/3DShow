#include "deviceswidget.h"

#include "config.h"

#include <QMenu>

DevicesWidget::DevicesWidget(QWidget *parent)
    : QListWidget(parent),
    m_pMenu(nullptr),
    m_pLastClickedItem(nullptr)
{
#ifdef COMMENT
    QStringList a;
    for (int i = 0; i < 4; ++i)
    {
        a << QString::number(i);
    }
    this->addItems(a);
#endif // COMMENT

    createMenu();
    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, &DevicesWidget::customContextMenuRequested, this, &DevicesWidget::slot_customContextMenuRequested);
    connect(this, &DevicesWidget::itemDoubleClicked, this, &DevicesWidget::slot_itemDoubleClicked);
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

    m_pSubMenu = new QMenu("Start Require", this);
    QAction *p = nullptr;

    p = new QAction("Show Color", this);
    connect(p, &QAction::triggered, [this] {
        informMultiAreaToCreateWidget(Utilities::ShowType::Color);
    });
    m_pSubMenu->addAction(p);

    p = new QAction("Show Depth", this);
    connect(p, &QAction::triggered, [this] {
        informMultiAreaToCreateWidget(Utilities::ShowType::Depth);
    });
    m_pSubMenu->addAction(p);

    p = new QAction("Show Skele", this);
    connect(p, &QAction::triggered, [this] {
        informMultiAreaToCreateWidget(Utilities::ShowType::Skele);
    });
    m_pSubMenu->addAction(p);

    p = new QAction("Show Color_Skele", this);
    connect(p, &QAction::triggered, [this] {
        informMultiAreaToCreateWidget(Utilities::ShowType::Color_Skele);
    });
    m_pSubMenu->addAction(p);

    p = new QAction("Show Depth_Skele", this);
    connect(p, &QAction::triggered, [this] {
        informMultiAreaToCreateWidget(Utilities::ShowType::Depth_Skele);
    });
    m_pSubMenu->addAction(p);

    p = new QAction("Show Color_Depth", this);
    connect(p, &QAction::triggered, [this] {
        informMultiAreaToCreateWidget(Utilities::ShowType::Color_Depth);
    });
    m_pSubMenu->addAction(p);

    p = new QAction("Show Color_Depth_Skele", this);
    connect(p, &QAction::triggered, [this] {
        informMultiAreaToCreateWidget(Utilities::ShowType::Color_Depth_Skele);
    });
    m_pSubMenu->addAction(p);
    p = nullptr;

    m_pMenu->addMenu(m_pSubMenu);

    m_pEndRequire = new QAction("End Require", this);
    connect(m_pEndRequire, &QAction::triggered, [this] {
        if (m_pLastClickedItem)
        {
            QString strWindowTitle = m_pLastClickedItem->text();
            emit signal_closeShowWidget(strWindowTitle);
        }
    });

    //m_pMenu->addAction(m_pEndRequire);

}

void DevicesWidget::informMultiAreaToCreateWidget(Utilities::ShowType type)
{
    if (m_pLastClickedItem)
    {
        QString strWindowTitle = m_pLastClickedItem->text();
        emit signal_createShowWidget(strWindowTitle, type);
    }
}

void DevicesWidget::slot_customContextMenuRequested(QPoint point)
{
    m_pLastClickedItem = this->itemAt(point);

    if (m_pLastClickedItem)
    {
        m_pMenu->clear();
        if (Config::GetInstance()->getDeviceStatus(m_pLastClickedItem->text()))
        {
            m_pMenu->addAction(m_pEndRequire);
        }
        else
        {
            m_pMenu->addMenu(m_pSubMenu);
        }
        m_pMenu->exec(QCursor::pos());
    }
}

void DevicesWidget::slot_endRequireData()
{
}

void DevicesWidget::slot_itemDoubleClicked(QListWidgetItem * item)
{
    if (item)
    {
        emit signal_showShowWidget(item->text());
    }
}

void DevicesWidget::slot_setDevices(const QStringList& devicesList)
{
    this->clear();
    this->addItems(devicesList);
}