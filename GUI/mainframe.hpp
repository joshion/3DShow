#pragma once
#include "mainwindow.hpp"

#include <QObject>

class OrderSocketThread;

class MainFrame : public QObject
{
	Q_OBJECT

public:
	MainFrame(QObject * parent = Q_NULLPTR);
	~MainFrame();

private:
    MainWindow *m_pMainWindow = nullptr;
    OrderSocketThread *m_pOrderSocketThread = nullptr;
};
