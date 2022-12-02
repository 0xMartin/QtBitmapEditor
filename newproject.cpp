#include "newproject.h"
#include "ui_newproject.h"

NewProject::NewProject(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::NewProject)
{
    ui->setupUi(this);
}

NewProject::~NewProject()
{
    delete ui;
}
