#pragma once
#include <QWidget>
#include "ui_mainwindow.h"

class MainFrame;

class MainWindow : public QWidget 
{
	Q_OBJECT

public:
	MainWindow(QWidget * parent = Q_NULLPTR);
	~MainWindow();

private:
	Ui::MainWindow ui;

signals: void requireConnect();
signals: void exitConnect();
    //void requireDevices();
    //void startConnect(std::string deviceName, unsigned int dataType);
signals: void endConnect();
};
