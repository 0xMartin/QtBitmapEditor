#include "mainwindow.h"

#include <QApplication>
#include <QFile>

#define DEBUG

#ifndef DEBUG
#include <QSplashScreen>
#include <QTimer>
#endif


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // spash screen
#ifndef DEBUG
    QPixmap pixmap(":/src/splash/splash.png");
    QSplashScreen splash(pixmap);
    splash.show();
#endif

    // nacteni theme
    QFile theme(":/src/theme/Combinear.qss");
    if(theme.open(QFile::ReadOnly)) {
        qDebug() << "Theme load ... ok";
    } else {
        qDebug() << "Theme load ... fail";
    }
    app.setStyleSheet(theme.readAll());

    // zakladni nastaveni aplikace
    app.setOrganizationName("UTB");
    app.setOrganizationDomain("utb.cz");
    app.setApplicationName("QtBitmapEditor");

    // hlavni okno aplikace
    MainWindow window;
    window.setWindowState(Qt::WindowState::WindowActive);
#ifdef DEBUG
    window.show();
#endif

    // spusteni aplikace
#ifndef DEBUG
    QTimer::singleShot(1500, &splash, SLOT(close()));
    QTimer::singleShot(1500, &window, SLOT(show()));
#endif
    return app.exec();
}
