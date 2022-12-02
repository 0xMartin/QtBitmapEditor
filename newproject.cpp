#include "newproject.h"
#include "ui_newproject.h"

#include <QMessageBox>
#include <QFileDialog>

#include "objects/project.h"


NewProject::NewProject(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewProject)
{
    ui->setupUi(this);
    this->setWindowTitle("New Project");
}

NewProject::~NewProject()
{
    delete ui;
}

void NewProject::setComponents(AppContext_t *context)
{
    this->context = context;
}

void NewProject::on_pushButton_clicked()
{
    // vytvoreni noveho projektu
    if(this->context == NULL) {
        qDebug() << "NewProject: app context is NULL";
        return;
    }

    // nacteni a overeni nazvu projektu
    QString name = this->ui->lineEdit_name->text();
    if(name.length() < PROJECT_MIN_NAME_LENGTH) {
       QMessageBox::warning(
                   this,
                   tr("New Project"),
                   tr("Name of the project is too short. The name must have at least 3 characters."));
       return;
    }

    // nacteni a overeni cesty projektu
    QString path = this->ui->lineEdit_path->text();
    if(path.length() == 0) {
       QMessageBox::warning(
                   this,
                   tr("New Project"),
                   tr("Path of project is not set"));
       return;
    }
    QFile file(path);
    if (file.exists())
    {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(
                    this,
                    tr("New Project"),
                    tr("Name of the project is same as the name of existing file. Do you want to create a project?"),
                    QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::No) {
            return;
        }
    }

    // nacteni velikosti obrazku
    int width = this->ui->spinBox_width->value();
    int height = this->ui->spinBox_height->value();

    // vytvori novy projekt a priradi ho do workspacu
    Project *p = new Project(name, path, QSize(width, height));
    AptCntx_setProject(this->context, p);

    // info + zavreni okna
    QMessageBox::information(this, tr("New Project"), tr("New project successfully created"));
    this->close();
}


void NewProject::on_pushButton_path_clicked()
{
    // zobrazeni file dialogu pro umisteni projektoveho souboru
    QString path = QFileDialog::getSaveFileName(
                this,
                "Save file",
                QDir::homePath(), PROJECT_FILE_EXTENSION);
    this->ui->lineEdit_path->setText(path + PROJECT_FILE_EXTENSION);
}

