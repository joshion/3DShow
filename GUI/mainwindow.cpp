﻿#include "mainwindow.hpp"

MainWindow::MainWindow(QWidget * parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    connect(ui.pushButton, &QPushButton::clicked, this, &MainWindow::requireConnect);
    connect(ui.pushButton_2, &QPushButton::clicked, this, &MainWindow::exitConnect);
    connect(ui.pushButton_3, &QPushButton::clicked, this, &MainWindow::requireDevices);
    connect(ui.pushButton_4, &QPushButton::clicked, this, &MainWindow::slot_startConnect);
    connect(ui.pushButton_5, &QPushButton::clicked, this, &MainWindow::endConnect);

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
    exit(0);
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
    emit startRequire("aaa", 1);
}
