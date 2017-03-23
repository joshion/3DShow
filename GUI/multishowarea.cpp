#include "multishowarea.h"
#include "skeletonframewidget.h"

MultiShowArea::MultiShowArea(QWidget *parent)
    : QMdiArea(parent)
{
    SkeletonFrameWidget *p = new SkeletonFrameWidget { 7893 };
    p->setAttribute(Qt::WA_DeleteOnClose);
    this->addSubWindow(p, Qt::Widget);
    p->showMaximized();
}

MultiShowArea::~MultiShowArea()
{

}
