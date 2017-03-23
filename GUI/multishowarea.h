#pragma once
#include <QMdiArea>

class MultiShowArea : public QMdiArea
{
    Q_OBJECT

public:
    MultiShowArea(QWidget *parent = 0);
    ~MultiShowArea();

};
