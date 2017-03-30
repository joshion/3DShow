#pragma once
#include "utilities.h"
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
    QAction *m_pStartRequire_Color;
    QAction *m_pStartRequire_Depth;
    QAction *m_pStartRequire_Skele;

    QAction *m_pStartRequire_Color_Skele;
    QAction *m_pStartRequire_Depth_Skele;
    QAction *m_pStartRequire_Color_Depth;

    QAction *m_pStartRequire_Color_Depth_Skele;

    QAction *m_pEndRequireData;

    QListWidgetItem *m_pLastClickedItem;

signals:
    void signal_createShowWidget(QString strWindowTile, Utilities::ShowType type);
public slots:
    void slot_setDevices(const QStringList& devicesList);
private slots:
    void slot_customContextMenuRequested(QPoint point);
    void slot_startRequire_Color();
    void slot_startRequire_Depth();
    void slot_startRequire_Skele();
    void slot_startRequire_Color_Skele();
    void slot_startRequire_Depth_Skele();
    void slot_startRequire_Color_Depth();
    void slot_startRequire_Color_Depth_Skele();
    void slot_endRequireData();
};
