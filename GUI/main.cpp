#include <mainframe.hpp>

#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainFrame mainframe;
    return a.exec();
}
