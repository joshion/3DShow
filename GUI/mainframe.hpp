#pragma once

#include <QObject>

class MainWindow;
class OrderSocketThread;

class MainFrame : public QObject
{
    Q_OBJECT

public:
    MainFrame(QObject * parent = Q_NULLPTR);
    ~MainFrame();

public:
    MainWindow* getMainWindowPtr()
    {
        return m_pMainWindow;
    }
private:
    MainWindow *m_pMainWindow;
    OrderSocketThread *m_pOrderSocketThread;
};
