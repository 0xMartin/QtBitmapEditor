#include "mainwindow.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFile theme(":/src/theme/Combinear.qss");
    if(theme.open(QFile::ReadOnly)) {
        qDebug() << "Theme load ... ok";
    } else {
        qDebug() << "Theme load ... fail";
    }
    a.setStyleSheet(theme.readAll());

    QCoreApplication::setOrganizationName("UTB");
    QCoreApplication::setOrganizationDomain("utb.cz");
    QCoreApplication::setApplicationName("QtBitmapEditor");

    MainWindow w;
    w.show();
    return a.exec();
}
