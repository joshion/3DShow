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
    QAction *m_pStartRequireRgb;
    QAction *m_pStartRequireDepth;
    QAction *m_pStartRequireSkeleton;
    QAction *m_pEndRequireData;

    QListWidgetItem *m_pLastClickedItem;

signals:
    void signal_createSkeletonFrameWidget(QString strWindowTile, unsigned int uPort);
public slots:
    void slot_setDevices(const QStringList& devicesList);
private slots:
    void slot_customContextMenuRequested(QPoint point);
    void slot_startRequireRgb();
    void slot_startRequireDepth();
    void slot_startRequireSkeleton();
    void slot_endRequireData();
};
