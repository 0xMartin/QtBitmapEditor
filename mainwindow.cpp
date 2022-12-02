#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    /*****************************************************************************/
    // zakladni inicializace UI
    this->ui->setupUi(this);
    // setaveni toolbaru
    this->colorPicker = new ColorPicker(this->ui->toolBar);
    this->ui->toolBar->addWidget(this->colorPicker);
    this->ui->toolBar->addAction(this->ui->actionZoom_in);
    this->ui->toolBar->addAction(this->ui->actionZoom_out);
    this->ui->toolBar->addSeparator();
    this->ui->toolBar->addAction(this->ui->actionPen);
    this->ui->toolBar->addAction(this->ui->actionEraser);
    this->ui->toolBar->addAction(this->ui->actionBrush);
    this->ui->toolBar->addAction(this->ui->actionFill);
    this->ui->toolBar->addAction(this->ui->actionText);
    this->ui->toolBar->addSeparator();
    this->ui->toolBar->addAction(this->ui->actionCircle);
    this->ui->toolBar->addAction(this->ui->actionRectangle);
    this->ui->toolBar->addAction(this->ui->actionPolygon);
    this->ui->toolBar->addSeparator();
    // scroll area pro workspace
    this->scrollArea_workspace = new QScrollArea(this);
    /*****************************************************************************/


    /*****************************************************************************/
    // inicializace hlavnich komponent
    this->workspace = new Workspace(this->scrollArea_workspace);
    this->toolController = new ToolController(this);
    this->layerManager = new LayerManager(this);
    /*****************************************************************************/


    /*****************************************************************************/
    // sestaveni celkove pracovni plochy s vyuzitim splitteru
    this->splitter_horizontal = new QSplitter(Qt::Horizontal);

    // leva strana (pracovani plocha pro upravu bitmapove grafiky)
    this->scrollArea_workspace->setWidget(this->workspace);
    this->splitter_horizontal->addWidget(this->scrollArea_workspace);

    // prava strany (manazer vrstev + ovladani aktualne vybraneho nastroje)
    this->splitter_vertical = new QSplitter(Qt::Vertical);
    // horni strana (ovladani nastroje)
    this->splitter_vertical->addWidget(this->toolController);
    // dolni strana (manazer vrstev)
    this->splitter_vertical->addWidget(this->layerManager);

    this->splitter_horizontal->addWidget(this->splitter_vertical);
    this->setCentralWidget(this->splitter_horizontal);
    /*****************************************************************************/


    /*****************************************************************************/
    // inicializace oken
    this->window_newProject = new NewProject();
    /*****************************************************************************/


    this->statusBar()->showMessage("Project: None");
}

MainWindow::~MainWindow()
{
    delete ui;

    if(workspace) delete workspace;
    if(toolController) delete toolController;
    if(layerManager) delete layerManager;

    if(colorPicker) delete colorPicker;

    if(splitter_horizontal) delete splitter_horizontal;
    if(splitter_vertical) delete splitter_vertical;
    if(scrollArea_workspace) delete scrollArea_workspace;
}


void MainWindow::on_actionNew_project_triggered()
{
    this->window_newProject->show();
}


void MainWindow::on_actionOpen_project_triggered()
{

}


void MainWindow::on_actionImport_image_triggered()
{

}


void MainWindow::on_actionExport_image_triggered()
{

}


void MainWindow::on_actionExit_triggered()
{

}


void MainWindow::on_actionPrint_triggered()
{

}


void MainWindow::on_actionAbout_triggered()
{

}


void MainWindow::on_actionPen_triggered()
{

}


void MainWindow::on_actionBrush_triggered()
{

}


void MainWindow::on_actionFill_triggered()
{

}


void MainWindow::on_actionText_triggered()
{

}


void MainWindow::on_actionRectangle_triggered()
{

}


void MainWindow::on_actionCircle_triggered()
{

}


void MainWindow::on_actionPolygon_triggered()
{

}


void MainWindow::on_actionEraser_triggered()
{

}


void MainWindow::on_actionAdd_layer_triggered()
{

}


void MainWindow::on_actionRemove_layer_triggered()
{

}


