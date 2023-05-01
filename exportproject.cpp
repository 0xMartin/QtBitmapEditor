#include "exportproject.h"
#include "ui_exportproject.h"

#include <QFileDialog>
#include <QPainter>
#include <QImage>
#include <QPixmap>
#include <QMessageBox>

#include "base/dialogs.h"


#define JPEG_FILES "JPG files (*.jpg)"
#define BMP_FILES "BMP files (*.bmp)"
#define PNG_FILES "PNG files (*.png)"



ExportProject::ExportProject(AppContext *context, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ExportProject)
{
    ui->setupUi(this);
    this->context = context;
    this->scale = 1.0;

    this->paintPreview();
}

ExportProject::~ExportProject()
{
    delete ui;
}

void ExportProject::paintPreview()
{
    if(this->context == NULL) return;
    Project *p = this->context->getProject();
    if(p == NULL) return;

    const int width = 500;
    const int height = 500;
    int c_w = height * ((float)p->getSize().width() / p->getSize().height());
    int c_h = height;
    if(c_w > width) {
        c_w = width;
        c_h = height * ((float)p->getSize().height() / p->getSize().width());
    }
    QPixmap image(QSize(c_w, c_h));
    QPainter painter;
    painter.begin(&image);
    float scale = (float)image.width() / qMax(c_w, p->getSize().width());
    if(p->getSize().width() < height && p->getSize().height()) {
        scale *= (float)height / qMax(p->getSize().width(), p->getSize().height());
    }
    scale *= this->scale;
    painter.scale(scale, scale);
    p->paintEvent(painter);
    painter.end();

    this->ui->labelImagePreview->setPixmap(image);
}

void ExportProject::on_pushButton_export_clicked()
{
    if(this->context == NULL) return;
    Project *p = this->context->getProject();
    if(p == NULL) {
        QMessageBox::warning(
                    this,
                    tr("Export Project"),
                    DIALOG_PROJECT_NOT_EXISTS);
        return;
    }

    QString path =  this->ui->lineEdit_path->text();
    if(path.length() == 0) {
       QMessageBox::warning(
                   this,
                   tr("Export Project"),
                   tr("Path of image is not set"));
       return;
    }
    QFile file(path);
    if (file.exists())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(
                    this,
                    tr("Export Project"),
                    tr("Name of the exported file is same as the name of existing file. Do you really want to overwrite the file?"),
                    QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::No) {
            return;
        }
    }

    // paint
    QImage image(p->getSize(), QImage::Format_ARGB32);
    QPainter painter;
    painter.begin(&image);
    p->paintEvent(painter, true);
    painter.end();

    // save image
    image.save(path);

    // info + zavreni okna
    QMessageBox::information(this, tr("Export Project"), tr("Project successfully exported"));
    this->close();
}


void ExportProject::on_pushButton_path_clicked()
{
    QString filter = QString("%1 ;; %2 ;; %3").arg(PNG_FILES, JPEG_FILES, BMP_FILES);
    QString selectedFilter;
    QString path = QFileDialog::getSaveFileName(
                this,
                "Export Project",
                QDir::homePath(), filter, &selectedFilter);

    QString suffix = "";
    if(selectedFilter == PNG_FILES) {
        suffix = ".png";
    } else if(selectedFilter == JPEG_FILES) {
        suffix = ".jpg";
    } else {
        suffix = ".bmp";
    }

    if(path.endsWith(suffix)) {
        this->ui->lineEdit_path->setText(path);
    } else {
        this->ui->lineEdit_path->setText(path + suffix);
    }
}

