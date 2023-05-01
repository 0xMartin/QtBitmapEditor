#include "openproject.h"
#include "ui_openproject.h"

#include <QMessageBox>
#include <QFileDialog>

#include "base/project.h"
#include "layer/bitmaplayer.h"


OpenProject::OpenProject(AppContext *context, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OpenProject)
{
    this->context = context;
    ui->setupUi(this);
    this->setWindowTitle("New Project");
}

OpenProject::~OpenProject()
{
    delete ui;
}

void OpenProject::on_pushButton_clicked()
{
    if(this->context == NULL) {
        qDebug() << "Open Project: app context is NULL";
        return;
    }

    // nacteni a overeni cesty projektu
    QString path = this->ui->lineEdit_path->text();
    if(path.length() == 0) {
       QMessageBox::warning(
                   this,
                   tr("Open Project"),
                   tr("Path of project is not set"));
       return;
    }

    // vytvori novy projekt a priradi ho do workspacu
    Project *p = ReadProjectFromFile(path);
    if(p == NULL) {
        QMessageBox::warning(
                    this,
                    tr("Open Project"),
                    tr("Failed to open project"));
        return;
    }
    this->context->setProject(p);

    // info + zavreni okna
    QMessageBox::information(this, tr("Open Project"), tr("Project successfully opened"));
    this->close();

    // singnal -> project vytvoren
    emit this->projectOpened();
}


void OpenProject::on_pushButton_path_clicked()
{
    // zobrazeni file dialogu pro umisteni projektoveho souboru
    QString filter = QString("QT-BE Project (*%1)").arg(PROJECT_FILE_EXTENSION);
    QString path = QFileDialog::getOpenFileName(
                this,
                "Open Project",
                QDir::homePath(), filter);
    this->ui->lineEdit_path->setText(path);
}

