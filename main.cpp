#include "projectwizard.h"


#include <QApplication>
#include <QFile>

#include "base/app_context.h"


#define DEBUG

#ifndef DEBUG
#include <QSplashScreen>
#include <QTimer>
#endif


AppContext *context;


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


    // vytvoreni kontextu aplikace
    context = new AppContext();


    // project wizard
    ProjectWizard window(context);
    window.setWindowState(Qt::WindowState::WindowActive);


#ifdef DEBUG
    window.show();
#endif

    // spusteni aplikace
#ifndef DEBUG
    QTimer::singleShot(1800, &splash, SLOT(close()));
    QTimer::singleShot(1800, &window, SLOT(show()));
#endif
    return app.exec();
}
