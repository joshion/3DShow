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
signals: void requireDevices();
signals: void startRequire(std::string deviceName, unsigned int dataType);
signals: void endConnect();

private slots:
    void slots_startConnect();
};
