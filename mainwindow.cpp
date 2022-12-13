#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "layer/bitmaplayer.h"
#include "tool/pen.h"
#include "tool/eraser.h"
#include "tool/brush.h"


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
    // label pro status bar
    this->statusLabel = new QLabel(this->ui->statusbar);
    this->statusLabel->setAlignment(Qt::AlignLeft);
    this->statusLabel->setText(tr("Project: None"));
    this->statusBar()->addPermanentWidget(this->statusLabel, 1);
    /*****************************************************************************/


    /*****************************************************************************/
    // inicializace kontextu
    this->context = new AppContext();
    this->context->setWorkspace(new Workspace(Workspace_defaultConfig(), this));
    this->context->setToolController(new ToolController(this));
    this->context->setLayerManager(new LayerManager(this));
    this->context->addTool(new Pen(this, this->colorPicker));
    this->context->addTool(new Eraser(this));
    this->context->addTool(new Brush(this, this->colorPicker));
    this->context->selectToolFromList(TOOL_PEN);
    /*****************************************************************************/


    /*****************************************************************************/
    // sestaveni celkove pracovni plochy s vyuzitim splitteru
    this->splitter_horizontal = new QSplitter(Qt::Horizontal);
    this->splitter_horizontal->setObjectName("bg-widget");
    // leva strana (pracovani plocha pro upravu bitmapove grafiky)
    this->splitter_horizontal->addWidget(this->context->getWorkspace());

    // prava strany (manazer vrstev + ovladani aktualne vybraneho nastroje)
    this->splitter_vertical = new QSplitter(Qt::Vertical);
    this->splitter_vertical->setObjectName("bg-widget");
    // horni strana (ovladani nastroje)
    this->splitter_vertical->addWidget(this->context->getToolController());
    // dolni strana (manazer vrstev)
    this->splitter_vertical->addWidget(this->context->getLayerManager());

    this->splitter_horizontal->addWidget(this->splitter_vertical);
    this->setCentralWidget(this->splitter_horizontal);
    /*****************************************************************************/


    /*****************************************************************************/
    // inicializace oken
    this->window_newProject = new NewProject();
    this->window_newProject->setContext(this->context);
    /*****************************************************************************/


    /*****************************************************************************/
    // vlastni styly
    this->splitter_horizontal->setStyleSheet("QWidget#bg-widget {background: rgb(49, 49, 49);}");
    /*****************************************************************************/


    // TEST
    QSize s = QSize(900, 500);
    Project *p = new Project(NULL, "Test", "/home/martin/aaa.qbe", s);
    BitmapLayer *l = new BitmapLayer(p, "Layer 1", s);
    p->addLayer(l);
    p->setSelectedLayer(l);
    this->context->setProject(p);

    // finish
    this->updateStatusBar();
    this->highlightToolbar(this->ui->actionPen);
}

MainWindow::~MainWindow()
{
    delete ui;

    delete this->context;

    if(this->colorPicker) delete this->colorPicker;
    if(this->splitter_horizontal) delete this->splitter_horizontal;
    if(this->statusLabel) delete this->statusLabel;
}

void MainWindow::updateStatusBar()
{
    if(this->context->getWorkspace() == NULL) return;
    Project *p = this->context->getWorkspace()->getProject();
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
    if(this->context == NULL) return;
    this->context->selectToolFromList(TOOL_PEN);
    this->highlightToolbar(this->ui->actionPen);
}


void MainWindow::on_actionBrush_triggered()
{
    if(this->context == NULL) return;
    this->context->selectToolFromList(TOOL_BRUSH);
    this->highlightToolbar(this->ui->actionBrush);
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
    if(this->context == NULL) return;
    this->context->selectToolFromList(TOOL_ERASER);
    this->highlightToolbar(this->ui->actionEraser);
}


void MainWindow::on_actionAdd_layer_triggered()
{

}


void MainWindow::on_actionRemove_layer_triggered()
{

}

void MainWindow::highlightToolbar(QAction *action)
{
    QWidget *w;
    for(QAction *a :this->ui->toolBar->actions()) {
        if(a != NULL) {
            w = this->ui->toolBar->widgetForAction(a);
            if(w != NULL && w != this->colorPicker) {
                w->setStyleSheet("");
            }
        }
    }
    w = this->ui->toolBar->widgetForAction(action);
    if(w != NULL) {
        w->setStyleSheet("background: rgba(183, 134, 32, 20%); border: 1px solid #b78620;");
    }
}

void MainWindow::on_actionZoom_in_triggered()
{
    if(this->context == NULL) return;
    if(this->context->getWorkspace() == NULL) return;
    this->context->getWorkspace()->zoomIN();
    this->context->getWorkspace()->repaint();
}


void MainWindow::on_actionZoom_out_triggered()
{
    if(this->context == NULL) return;
    if(this->context->getWorkspace() == NULL) return;
    this->context->getWorkspace()->zoomOUT();
    this->context->getWorkspace()->repaint();
}

