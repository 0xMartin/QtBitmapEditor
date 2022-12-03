#include "mainwindow.h"

#include <QApplication>
#include <QSplashScreen>
#include <QFile>
#include <QTimer>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // spash screen
    QPixmap pixmap(":/src/splash/splash.png");
    QSplashScreen splash(pixmap);
    splash.show();

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

    // spusteni aplikace
    QTimer::singleShot(150, &splash, SLOT(close()));
    QTimer::singleShot(150, &window, SLOT(show()));
    return app.exec();
}
