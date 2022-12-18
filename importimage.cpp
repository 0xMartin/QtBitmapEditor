#include "importimage.h"
#include "ui_importimage.h"

#include <QMessageBox>
#include <QFile>
#include <QFileDialog>

#include "base/dialogs.h"
#include "layer/imagelayer.h"


ImportImage::ImportImage(AppContext *context, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ImportImage)
{
    this->context = context;
    ui->setupUi(this);
}

ImportImage::~ImportImage()
{
    delete ui;
}

void ImportImage::on_pushButton_import_clicked()
{
    if(this->context == NULL) {
        qDebug() << "NewProject: app context is NULL";
        return;
    }

    // nacteni a overeni cesty projektu
    QString path = this->ui->lineEdit_path->text();
    if(path.length() == 0) {
       QMessageBox::warning(
                   this,
                   tr("Import Image"),
                   tr("Path to image is not set"));
       return;
    }

    // nacteni velikosti obrazku
    int width = this->ui->spinBox_width->value();
    int height = this->ui->spinBox_height->value();

    // nacteni a kontrola projektu
    Project *project = this->context->getProject();
    if(project == NULL) {
        QMessageBox::warning(
                    this,
                    tr("Import Image"),
                    DIALOG_PROJECT_NOT_EXISTS);
        return;
    }

    // vytvori vrstvu z obrakem
    ImageLayer *layer = new ImageLayer(project,
                                       "Image ",
                                       this->ui->lineEdit_path->text(),
                                       QSize(width, height)
                                       );
    project->insertLayerAbove(layer);
    project->requestRepaint();

    QMessageBox::information(this, tr("Import Image"), tr("Image successfully imported"));
    this->close();
}


void ImportImage::on_pushButton_path_clicked()
{
    // nacteni a kontrola projektu
    Project *project = this->context->getProject();
    if(project == NULL) {
        QMessageBox::warning(
                    this,
                    tr("Import Image"),
                    DIALOG_PROJECT_NOT_EXISTS);
        return;
    }

    // zobrazeni file dialogu pro import obrazku
    QString path = QFileDialog::getOpenFileName(
                this,
                "Open File",
                QDir::homePath(), tr("Image Files (*.png *.jpg *.bmp)"));

    // relativni cesta k projektovemu
    QDir dir(project->getDirPath());

    QString rel = dir.relativeFilePath(path);
    this->ui->lineEdit_path->setText(rel);
}

