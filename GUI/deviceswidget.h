#ifndef DEVICESWIDGET_H
#define DEVICESWIDGET_H

#include <QListWidget>

class DevicesWidget : public QListWidget
{
    Q_OBJECT

public:
    DevicesWidget(QWidget *parent = 0);
    ~DevicesWidget();
    
public slots:
    void slot_setDevices(const QStringList& devicesList);
};

#endif // DEVICESWIDGET_H
