#include "deviceswidget.h"

DevicesWidget::DevicesWidget(QWidget *parent)
    : QListWidget(parent)
{
}

DevicesWidget::~DevicesWidget()
{

}

void DevicesWidget::slot_setDevices(const QStringList& devicesList)
{
    this->clear();
    this->addItems(devicesList);
}
