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
    // label pro status bar
    this->statusLabel = new QLabel(this->ui->statusbar);
    this->statusLabel->setAlignment(Qt::AlignLeft);
    this->statusLabel->setText(tr("Project: None"));
    this->statusBar()->addPermanentWidget(this->statusLabel, 1);
    /*****************************************************************************/


    /*****************************************************************************/
    // inicializace kontextu
    this->context.workspace = new Workspace(this->scrollArea_workspace);
    this->context.toolController = new ToolController(this);
    this->context.layerManager = new LayerManager(this);
    /*****************************************************************************/


    /*****************************************************************************/
    // sestaveni celkove pracovni plochy s vyuzitim splitteru
    this->splitter_horizontal = new QSplitter(Qt::Horizontal);

    // leva strana (pracovani plocha pro upravu bitmapove grafiky)
    this->scrollArea_workspace->setWidget(this->context.workspace);
    this->splitter_horizontal->addWidget(this->scrollArea_workspace);

    // prava strany (manazer vrstev + ovladani aktualne vybraneho nastroje)
    this->splitter_vertical = new QSplitter(Qt::Vertical);
    // horni strana (ovladani nastroje)
    this->splitter_vertical->addWidget(this->context.toolController);
    // dolni strana (manazer vrstev)
    this->splitter_vertical->addWidget(this->context.layerManager);

    this->splitter_horizontal->addWidget(this->splitter_vertical);
    this->setCentralWidget(this->splitter_horizontal);
    /*****************************************************************************/


    /*****************************************************************************/
    // inicializace oken
    this->window_newProject = new NewProject();
    this->window_newProject->setComponents(&this->context);
    /*****************************************************************************/

}

MainWindow::~MainWindow()
{
    delete ui;

    destructAppContext(this->context);

    if(this->colorPicker) delete this->colorPicker;

    if(this->splitter_horizontal) delete this->splitter_horizontal;
    if(this->splitter_vertical) delete this->splitter_vertical;
    if(this->scrollArea_workspace) delete this->scrollArea_workspace;
    if(this->statusLabel) delete this->statusLabel;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(this->context.workspace == NULL) return;
    Project *p = this->context.workspace->getProject();
    if(p == NULL) {
        this->statusLabel->setText(tr("Project: None"));
    } else {
        this->statusLabel->setText(QString(tr("Project: %1 (%2)")).arg(p->getName(), p->getPath()));
    }
}

void MainWindow::on_actionNew_project_triggered()
{
    this->window_newProject->show();
}


void MainWindow::on_actionOpen_project_triggered()
{

}


void MainWindow::on_actionSave_project_triggered()
{
    this->statusLabel->setText(tr("Project saved"));
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


