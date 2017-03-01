#include "mainframe.hpp"

MainFrame::MainFrame(QObject * parent) 
    : QObject(parent) 
{
    m_pMainWindow = new MainWindow;
    m_pMainWindow->show();
}

MainFrame::~MainFrame()
{
    delete m_pMainWindow;
}
