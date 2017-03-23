#pragma once

#include <QListWidget>

class QMenu;

class DevicesWidget : public QListWidget
{
    Q_OBJECT

public:
    DevicesWidget(QWidget *parent = 0);
    ~DevicesWidget();
private:
    QMenu *m_pMenu;
    QAction *m_pStartRequireData;
    QAction *m_pEndRequireData;
public slots:
    void slot_setDevices(const QStringList& devicesList);
private slots:
    void slot_customContextMenuRequested(QPoint point);
};
