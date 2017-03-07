#include "mainwindow.hpp"

MainWindow::MainWindow(QWidget * parent)
    : QWidget(parent)
{
    ui.setupUi(this);
    connect(ui.pushButton, &QPushButton::clicked, this, &MainWindow::requireConnect);
    connect(ui.pushButton_2, &QPushButton::clicked, this, &MainWindow::exitConnect);
    connect(ui.pushButton_3, &QPushButton::clicked, this, &MainWindow::requireDevices);
    connect(ui.pushButton_4, &QPushButton::clicked, this, &MainWindow::slots_startConnect);
    connect(ui.pushButton_5, &QPushButton::clicked, this, &MainWindow::endConnect);
}

MainWindow::~MainWindow()
{
	
}

void MainWindow::slots_startConnect()
{
    emit startRequire("aaa", 1);
}
