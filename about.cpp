#include "about.h"
#include "ui_about.h"

#include <QDesktopServices>

About::About(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
}

About::~About()
{
    delete ui;
}

void About::on_label_9_linkActivated(const QString &link)
{
    QDesktopServices::openUrl(QUrl("https://github.com/0xMartin/QtBitmapEditor"));
}

