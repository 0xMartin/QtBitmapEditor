#include "projectfromimage.h"
#include "ui_projectfromimage.h"

#include <QMessageBox>
#include <QFileDialog>
#include <QImageReader>

#include "base/project.h"
#include "layer/bitmaplayer.h"


ProjectFromImage::ProjectFromImage(AppContext *context, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectFromImage)
{
    this->context = context;
    ui->setupUi(this);
    this->setWindowTitle("New Project");
}

ProjectFromImage::~ProjectFromImage()
{
    delete ui;
}

void ProjectFromImage::on_pushButton_clicked()
{
    if(this->context == NULL) {
        qDebug() << "Open Project: app context is NULL";
        return;
    }

    // nacteni a overeni nazvu projektu
    QString name = this->ui->lineEdit_name->text();
    if(name.length() < PROJECT_MIN_NAME_LENGTH) {
       QMessageBox::warning(
                   this,
                   tr("Project From Image"),
                   tr("Name of the project is too short. The name must have at least 3 characters."));
       return;
    }

    // nacteni a overeni cesty projektu
    QString path = this->ui->lineEdit_path->text();
    if(path.length() == 0) {
       QMessageBox::warning(
                   this,
                   tr("Project From Image"),
                   tr("Path of project is not set"));
       return;
    }
    QFile file(path);
    if (file.exists())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(
                    this,
                    tr("Project From Image"),
                    tr("Name of the project is same as the name of existing file. Do you want to create a project?"),
                    QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::No) {
            return;
        }
    }

    // nacteni a overeni cesty obrazku
    QString pathImg = this->ui->lineEdit_path_img->text();
    if(path.length() == 0) {
       QMessageBox::warning(
                   this,
                   tr("Project From Image"),
                   tr("Path of image is not set"));
       return;
    }

    // velikost obrazku
    QImageReader imgReader(pathImg);

    // vytvori novy projekt a priradi ho do workspacu
    Project *p = new Project(NULL, name, path, imgReader.size());
    BitmapLayer *l = new BitmapLayer(p, "Background", pathImg);
    p->addLayerAtTop(l);
    p->setSelectedLayer(l);
    this->context->setProject(p);

    // info + zavreni okna
    QMessageBox::information(this, tr("Open Project"), tr("Project successfully opened"));
    this->close();

    // singnal -> project vytvoren
    emit this->projectCreated();
}


void ProjectFromImage::on_pushButton_path_clicked()
{
    // zobrazeni file dialogu pro umisteni projektoveho souboru
    QString filter = QString("QT-BE Project (*%1)").arg(PROJECT_FILE_EXTENSION);
    QString path = QFileDialog::getSaveFileName(
                this,
                "Save Project",
                QDir::homePath(), filter);
    if(path.endsWith(PROJECT_FILE_EXTENSION)) {
        this->ui->lineEdit_path->setText(path);

    } else {
        this->ui->lineEdit_path->setText(path + PROJECT_FILE_EXTENSION);
    }
}


void ProjectFromImage::on_pushButton_path_2_clicked()
{
    // zobrazeni file dialogu pro otevreni obrazku
    QString path = QFileDialog::getOpenFileName(
                this,
                "Open Image",
                QDir::homePath(), tr("Image Files (*.png *.jpg *.bmp)"));
    this->ui->lineEdit_path_img->setText(path);
}

