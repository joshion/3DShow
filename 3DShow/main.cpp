
#ifdef _MSC_VER
#define _WIN32_WINNT 0x0501
#endif

#include "mainwindow.h"
#include <QtWidgets/QApplication>
#include "StreamPlayProto.pb.h"

#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include <boost/date_time/posix_time/posix_time.hpp>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    boost::asio::io_service io;

    boost::asio::deadline_timer *p = new boost::asio::deadline_timer(io, boost::posix_time::seconds(5));

    p->wait();

    MainWindow w;
    w.show();

    return a.exec();
}
