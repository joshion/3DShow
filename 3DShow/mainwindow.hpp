#pragma once
#include <QWidget>
#include "ui_mainwindow.h"

class MainWindow : public QWidget 
{
	Q_OBJECT

public:
	MainWindow(QWidget * parent = Q_NULLPTR);
	~MainWindow();

private:
	Ui::MainWindow ui;
};
