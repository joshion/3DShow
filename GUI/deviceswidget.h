#pragma once
#include "utilities.h"
#include "KinectDataProto.pb.h"
#include <QListWidget>

class QMenu;

class DevicesWidget : public QListWidget
{
    Q_OBJECT

public:
    DevicesWidget(QWidget *parent = 0);
    ~DevicesWidget();

private:
    void createMenu();
    void informMultiAreaToCreateWidget(Utilities::ShowType type);

private:
    QMenu *m_pMenu;
    QListWidgetItem *m_pLastClickedItem;

signals: void signal_createShowWidget(QString strWindowTile, Utilities::ShowType type);
signals: void signal_requireEndTransfer(KinectDataProto::pbReqEnd reqEnd);
public slots:
    void slot_setDevices(const QStringList& devicesList);
private slots:
    void slot_customContextMenuRequested(QPoint point);
    void slot_endRequireData();
};
