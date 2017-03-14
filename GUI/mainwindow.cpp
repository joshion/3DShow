#include "mainwindow.hpp"

MainWindow::MainWindow(QWidget * parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    connect(ui.m_ReqConnect, &QPushButton::clicked, this, &MainWindow::signal_requireConnect);
    connect(ui.m_ExitConnect, &QPushButton::clicked, this, &MainWindow::signal_exitConnect);
    connect(ui.m_ReqDevices, &QPushButton::clicked, this, &MainWindow::signal_requireDevices);
    connect(ui.m_StartConnect, &QPushButton::clicked, this, &MainWindow::slot_startConnect);
    connect(ui.m_EndConnect, &QPushButton::clicked, this, &MainWindow::signal_endConnect);

    connect(this, &MainWindow::signal_respConnect, this, &MainWindow::slot_respConnect);
    connect(this, &MainWindow::signal_respDevices, this, &MainWindow::slot_respDevices);
    connect(this, &MainWindow::signal_respStartRequire, this, &MainWindow::slot_respStartRequire);
    connect(this, &MainWindow::signal_reqEndConnect, this, &MainWindow::slot_reqEndConnect);
}

MainWindow::~MainWindow()
{
	
}

/*
主窗口关闭时,关闭所有线程,退出程序
*/
void MainWindow::closeEvent(QCloseEvent * event)
{
    this->close();
    emit signal_exitProcess();
}

void MainWindow::slot_respConnect()
{
    qDebug() << "enter" << __FILE__ << __LINE__ << "slot_respConnect";
}

void MainWindow::slot_respDevices()
{
    qDebug() << "enter" << __FILE__ << __LINE__ << "slot_respDevices";
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
