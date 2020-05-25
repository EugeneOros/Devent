#include "mainwindow.h"
#include "data.h"
#include <QApplication>
#include "../../SmtpClient-for-Qt/src/SmtpMime"



int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    MainWindow w;
    w.show();
    return a.exec();

}
