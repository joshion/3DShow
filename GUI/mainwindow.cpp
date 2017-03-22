#include "mainwindow.hpp"
#include <qdebug.h>
#include "deviceswidget.h"
#include "skeletonframewidget.h"

#include <QMdiSubWindow>

MainWindow::MainWindow(QWidget * parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    connect(ui.m_ReqConnect, &QPushButton::clicked, this, &MainWindow::signal_requireConnect);
    connect(ui.m_ExitConnect, &QPushButton::clicked, this, &MainWindow::signal_exitConnect);
    connect(ui.m_ReqDevices, &QPushButton::clicked, this, &MainWindow::signal_requireDevices);

    connect(ui.m_StartConnect, &QPushButton::clicked, this, &MainWindow::slot_startConnect);
    connect(ui.m_EndConnect, &QPushButton::clicked, this, &MainWindow::slot_buttonClicked);

    connect(this, &MainWindow::signal_respConnect, this, &MainWindow::slot_respConnect);
    connect(this, &MainWindow::signal_respDevices, this, &MainWindow::slot_respDevices);
    connect(this, &MainWindow::signal_respStartRequire, this, &MainWindow::slot_respStartRequire);
    connect(this, &MainWindow::signal_reqEndConnect, this, &MainWindow::slot_reqEndConnect);

    connect(this, &MainWindow::signal_respDevices, ui.m_DevicesWidget, &DevicesWidget::slot_setDevices);
}

MainWindow::~MainWindow()
{
	
}

void MainWindow::slot_buttonClicked()
{
    slot_createSkeletonFrameWidget("Kinect Show", 3434);
}

void MainWindow::slot_createSkeletonFrameWidget(QString strWindowTile, unsigned int uPort)
{
    // ui.m_MDIArea->clearMask();
    SkeletonFrameWidget *p = new SkeletonFrameWidget { uPort };
    p->setAttribute(Qt::WA_DeleteOnClose);
    ui.m_MDIArea->addSubWindow(p, Qt::Widget);
    p->showMaximized();

    // ui.m_MDIArea->cascadeSubWindows();
}

void MainWindow::slot_respConnect()
{
    qDebug() << "enter" << __FILE__ << __LINE__ << "slot_respConnect";
}

void MainWindow::slot_respDevices(const QStringList& devicesList)
{
    for (auto a : devicesList)
    {
        qDebug() << "enter" << __FILE__ << __LINE__ << "slot_respDevices" << a;
    }
}

void MainWindow::slot_respStartRequire()
{
    qDebug() << "enter" << __FILE__ << __LINE__ << "slot_respStartRequire";
}

void MainWindow::slot_reqEndConnect()
{
    qDebug() << "enter" << __FILE__ << __LINE__ << "slot_reqEndConnect";
}

void MainWindow::slot_startConnect()
{
    emit signal_startRequire("aaa", 1);
}
