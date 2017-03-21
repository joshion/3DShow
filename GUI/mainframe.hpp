#ifndef MAINFRAME_H
#define MAINFRAME_H

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

private:
    MainWindow *m_pMainWindow = nullptr;
    OrderSocketThread *m_pOrderSocketThread = nullptr;
};

#endif // !MAINFRAME_H
