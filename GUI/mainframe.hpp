#pragma once
#include "mainwindow.hpp"
#include <QObject>

class TcpSocket;

class MainFrame : public QObject 
{
	Q_OBJECT

public:
	MainFrame(QObject * parent = Q_NULLPTR);
	~MainFrame();

private:
    MainWindow *m_pMainWindow;
    TcpSocket *m_pSocket;
};
