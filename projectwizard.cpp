#include "projectwizard.h"
#include "ui_projectwizard.h"

#include "mainwindow.h"


ProjectWizard::ProjectWizard(AppContext *context, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ProjectWizard)
{
    this->context = context;
    ui->setupUi(this);
    this->setWindowTitle("Project Wizard");

    // novy projekt
    this->window_newProject = new NewProject(this->context);
    connect(this->window_newProject, SIGNAL(projectCreated()),
            this, SLOT(on_project_create()));

    // otevrit projekt
    this->window_openProject = new OpenProject(this->context);
    connect(this->window_openProject, SIGNAL(projectOpened()),
            this, SLOT(on_project_open()));

    // vytvori projekt z obrazku
    this->window_projectFromImage = new ProjectFromImage(this->context);
    connect(this->window_projectFromImage, SIGNAL(projectCreated()),
            this, SLOT(on_project_create()));

    this->hightLightButton(this->ui->pushButtonCreate);
    this->selectWidget(this->window_newProject);
}

ProjectWizard::~ProjectWizard()
{
    delete ui;

    if(this->window_newProject) delete this->window_newProject;
}

void ProjectWizard::on_pushButtonCreate_clicked()
{
    this->hightLightButton(this->ui->pushButtonCreate);
    this->selectWidget(this->window_newProject);
}


void ProjectWizard::on_pushButtonOpen_clicked()
{
    this->hightLightButton(this->ui->pushButtonOpen);
    this->selectWidget(this->window_openProject);
}


void ProjectWizard::on_pushButtonFromImage_clicked()
{
    this->hightLightButton(this->ui->pushButtonFromImage);
    this->selectWidget(this->window_projectFromImage);
}

void ProjectWizard::on_project_create()
{
    MainWindow *main = new MainWindow(this->context, NULL);
    main->showMaximized();
    this->close();
}

void ProjectWizard::on_project_open()
{
    MainWindow *main = new MainWindow(this->context, NULL);
    main->showMaximized();
    this->close();
}

void ProjectWizard::hightLightButton(QPushButton *button)
{
    this->ui->pushButtonCreate->setStyleSheet("");
    this->ui->pushButtonOpen->setStyleSheet("");
    this->ui->pushButtonFromImage->setStyleSheet("");
    if(button != NULL) {
        button->setStyleSheet("background: rgba(183, 134, 32, 20%); border: 1px solid #b78620;");
    }
}

void ProjectWizard::selectWidget(QWidget *widget)
{
    QLayoutItem *item = this->ui->verticalLayoutContainer->takeAt(0);
    if(item != NULL) {
        QWidget *w = item->widget();
        w->setParent(NULL);
    }
    if(widget != NULL) {
        this->ui->verticalLayoutContainer->addWidget(widget);
    }
    this->ui->widgetContainer->repaint();
}


