#include "layermanager.h"

#include <QMessageBox>
#include <QInputDialog>
#include <QPainter>
#include <QtMath>
#include <QDropEvent>

#include "dialogs.h"
#include "../layer/bitmaplayer.h"

/**********************************************************************************************/
// LayerListWidgetItem
/**********************************************************************************************/

LayerWidget::LayerWidget(Layer *layer, size_t height) : QWidget()
{
    this->height = height;
    this->layer = layer;
    if(this->layer == NULL) return;
    this->hBoxLayout = new QHBoxLayout(this);

    // ovladani viditelnosti vrstvy
    this->checkBox_visible = new QCheckBox(this);
    this->checkBox_visible->setFixedSize(QSize(height/2, height/2));
    this->checkBox_visible->setStyleSheet("QCheckBox::indicator { width:20px; height: 20px;}");
    this->checkBox_visible->setToolTip(tr("Visibility"));
    this->checkBox_visible->setChecked(this->layer->isVisible());
    connect(this->checkBox_visible, SIGNAL(toggled(bool)), this, SLOT(on_checkBox_visible_toggle(bool)));
    this->hBoxLayout->addWidget(this->checkBox_visible);

    // nahled vrstvy
    this->image = new QLabel(this);
    this->image->setFixedSize(QSize(height, height));
    this->image->setStyleSheet("border-width: 2px;");
    this->repaintLayer();
    this->hBoxLayout->addWidget(this->image);

    // nazev vrstvy
    this->label = new QLabel(layer->getName(), this);
    this->hBoxLayout->addWidget(this->label);

    // final
    this->setLayout(this->hBoxLayout);
}

LayerWidget::~LayerWidget() {
    if(this->hBoxLayout) delete this->hBoxLayout;
    if(this->image) delete this->image;
    if(this->label) delete this->label;
    if(this->checkBox_visible) delete this->checkBox_visible;
}

Layer *LayerWidget::getLayer() const
{
    return this->layer;
}

void LayerWidget::repaintLayer()
{
    QPixmap *pixmap = new QPixmap(height, height);
    if(pixmap) {
        QPainter painter(pixmap);
        float scale = (float)this->height / qMax(layer->getSize().width(), layer->getSize().height());
        Layer_paintBgGrid(painter, QPoint(0, 0), pixmap->size(),  pixmap->size(), 8);
        painter.scale(scale, scale);
        layer->paintEvent(painter);
        painter.end();
        this->image->setPixmap(*pixmap);
        delete pixmap;
    }
}

void LayerWidget::on_checkBox_visible_toggle(bool visible)
{
    if(this->layer) {
        this->layer->setVisible(visible);
        this->layer->requestRepaint();
    }
}


/**********************************************************************************************/
// LayerManager
/**********************************************************************************************/

LayerManager::LayerManager(QWidget *parent) : QWidget(parent)
{
    this->project = NULL;
    this->button_addLayer = NULL;
    this->button_removeLayer = NULL;
    this->button_up = NULL;
    this->button_down = NULL;
    this->setMaximumWidth(500);
    this->mainLayout = new QVBoxLayout(this);
    this->setLayout(this->mainLayout);


    // header
    //------------------------------------------------------------------------------------------
    this->header = new QLabel(this);
    this->header->setText(tr("Layers"));
    this->header->setStyleSheet("background: rgb(43, 43, 43); color: rgb(224, 224, 224); padding: 8px; border-bottom: 1px solid black");
    this->mainLayout->addWidget(this->header);


    // ovladani aktualni vrstvy (opacity)
    //------------------------------------------------------------------------------------------
    this->layerControl = new QWidget(this);
    this->mainLayout->addWidget(this->layerControl);
    this->layerControllLayout = new QHBoxLayout(this->layerControl);
    this->layerControl->setLayout(this->layerControllLayout);

    // opacity
    this->spinbox_opacity = new QSpinBox(this);
    //connect(this->spinbox_opacity, SIGNAL(clicked()), this, SLOT(on_button_addLayer_clicked()));
    this->spinbox_opacity->setToolTip(QString(tr("Opacity")));
    this->spinbox_opacity->setPrefix("Opacity: ");
    this->spinbox_opacity->setSuffix("%");
    this->spinbox_opacity->setMaximum(100);
    this->spinbox_opacity->setMinimum(0);
    this->spinbox_opacity->setValue(100);
    this->layerControllLayout->addWidget(this->spinbox_opacity);


    // list s vrstvama
    //------------------------------------------------------------------------------------------
    this->listWidget = new QListWidget(this);
    connect(this->listWidget, SIGNAL(itemSelectionChanged()),
            this, SLOT(on_listWidget_itemSelectionChanged()));
    this->mainLayout->addWidget(this->listWidget);


    // tlaciky pro ovladani listu vrstev (add, remove, up, down)
    //------------------------------------------------------------------------------------------
    this->listControl = new QWidget(this);
    this->mainLayout->addWidget(this->listControl);
    this->listControlLayout = new QHBoxLayout(this->listControl);
    this->listControl->setLayout(this->listControlLayout);

    //spacer
    this->listControlLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));

    // add layer tlacitko
    this->button_addLayer = new QPushButton();
    connect(this->button_addLayer, SIGNAL(clicked()), this, SLOT(on_button_addLayer_clicked()));
    this->button_addLayer->setToolTip(QString(tr("Add layer")));
    this->button_addLayer->setIcon(QIcon(":/src/icons/new_layer.png"));
    this->button_addLayer->setIconSize(QSize(22, 22));
    this->listControlLayout->addWidget(this->button_addLayer);

    // remove  tlacitko
    this->button_removeLayer = new QPushButton();
    connect(this->button_removeLayer, SIGNAL(clicked()), this, SLOT(on_button_removeLayer_clicked()));
    this->button_removeLayer->setToolTip(QString(tr("Remove layer")));
    this->button_removeLayer->setIcon(QIcon(":/src/icons/remove_layer.png"));
    this->button_removeLayer->setIconSize(QSize(22, 22));
    this->listControlLayout->addWidget(this->button_removeLayer);

    // up tlacitko
    this->button_up = new QPushButton();
    connect(this->button_up, SIGNAL(clicked()), this, SLOT(on_button_up_clicked()));
    this->button_up->setToolTip(QString(tr("Move layer up")));
    this->button_up->setIcon(QIcon(":/src/icons/arrow_up.png"));
    this->button_up->setIconSize(QSize(22, 22));
    this->listControlLayout->addWidget(this->button_up);

    // down tlacitko
    this->button_down = new QPushButton();
    connect(this->button_down, SIGNAL(clicked()), this, SLOT(on_button_down_clicked()));
    this->button_down->setToolTip(QString(tr("Move layer down")));
    this->button_down->setIcon(QIcon(":/src/icons/arrow_down.png"));
    this->button_down->setIconSize(QSize(22, 22));
    this->listControlLayout->addWidget(this->button_down);
}

LayerManager::~LayerManager() {
    if(this->header) delete this->header;
    if(this->spinbox_opacity) delete this->spinbox_opacity;
    if(this->listWidget) delete this->listWidget;
    if(this->button_addLayer) delete this->button_addLayer;
    if(this->button_removeLayer) delete this->button_removeLayer;
    if(this->button_up) delete this->button_up;
    if(this->button_down) delete this->button_down;
    if(this->mainLayout) delete this->mainLayout;
    if(this->listControl) delete this->listControl;
    if(this->layerControl) delete this->layerControl;
}

void LayerManager::setProject(Project *project)
{
    this->project = project;
    this->updateLayerList();
    connect(this->project, SIGNAL(repaintSignal(Layer*)), this, SLOT(on_project_repaintSignal(Layer*)));
}

Project *LayerManager::getProject() const
{
    return this->project;
}

void LayerManager::updateLayerList()
{
    if(this->listWidget == NULL) return;

    // clear
    this->listWidget->clear();

    // pro kazdu vrstvu projektu vlozi do listu item a pro nej take nastavi widget
    Layers_t *layers = this->project->getLayers();
    if(layers == NULL) return;

    for(auto it = layers->crbegin() ; it != layers->crend(); ++it) {
        LayerWidget *w = new LayerWidget(*it, 60); // dealokace pri clear
        QListWidgetItem *item = new QListWidgetItem(listWidget); // dealokace pri clear
        item->setSizeHint(QSize(80, 80));
        this->listWidget->insertItem(0, item);
        this->listWidget->setItemWidget(item, w);
        // oznaceni teto vrstvy ?
        if(w->getLayer() == this->project->getSelectedLayer()) {
            this->listWidget->setCurrentItem(item);
        }
    }
}

void LayerManager::changeEvent(QEvent *)
{
    // vzdy fixni velikost
    if(this->button_addLayer) this->button_addLayer->setFixedSize(QSize(30, 30));
    if(this->button_removeLayer) this->button_removeLayer->setFixedSize(QSize(30, 30));
    if(this->button_up) this->button_up->setFixedSize(QSize(30, 30));
    if(this->button_down) this->button_down->setFixedSize(QSize(30, 30));
}

void LayerManager::on_project_repaintSignal(Layer *layer)
{
    if(layer == NULL) return;
    QList<QListWidgetItem*> items = this->listWidget->selectedItems();
    LayerWidget *widget;
    for(QListWidgetItem *item : items) {
        widget = (LayerWidget*)this->listWidget->itemWidget(item);
        if(widget == NULL) continue;
        if(widget->getLayer() == layer) {
            widget->repaintLayer();
            widget->repaint();
            break;
        }
    }
}

void LayerManager::on_button_addLayer_clicked()
{
    if(this->project == NULL) {
        QMessageBox::warning(
                    this,
                    tr("Add layer"),
                    DIALOG_PROJECT_NOT_EXISTS);
        return;
    }

    bool ok;
    QString text = QInputDialog::getText(
                this,
                tr("Add layer"),
                tr("Layer name:"),
                QLineEdit::Normal,
                "Layer " + QString::number(this->project->getLayers()->size() + 1),
                &ok);
    if (ok && !text.isEmpty()) {
        // vytvoreni vrstvy a pridani do projektu
        Layer *layer = new BitmapLayer(this->project, text, this->project->getSize());
        this->project->addLayer(layer);
        this->project->setSelectedLayer(layer);
        // repaint projektu
        this->project->requestRepaint();
        // repaint listview
        this->updateLayerList();
    }
}

void LayerManager::on_button_removeLayer_clicked()
{
    if(this->project == NULL) {
        QMessageBox::warning(
                    this,
                    tr("Remove layer"),
                    DIALOG_PROJECT_NOT_EXISTS);
        return;
    }

    Layer *l = this->project->getSelectedLayer();
    if(l == NULL) {
        QMessageBox::warning(
                    this,
                    tr("Remove layer"),
                    DIALOG_NO_LAYER);
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
                this,
                tr("Remove layer"),
                QString(tr("Do you want to delete the layer named [%1]?")).arg(l->getName()),
                QMessageBox::Yes|QMessageBox::No);

    if(reply == QMessageBox::Yes) {
        this->project->removeLayer(l);
        // update layer listu
        this->updateLayerList();
        // prekresleni projektu
        this->project->requestRepaint();
    }
}

void LayerManager::on_button_up_clicked()
{
    if(this->project == NULL) {
        QMessageBox::warning(
                    this,
                    tr("Move layer up"),
                    DIALOG_PROJECT_NOT_EXISTS);
        return;
    }

    Layer *l = this->project->getSelectedLayer();
    if(l == NULL) {
        QMessageBox::warning(
                    this,
                    tr("Move layer up"),
                    DIALOG_NO_LAYER);
        return;
    }

    // presun o jedno nahoru
    this->project->moveSelectedLayerUp();
    // update layer listu
    this->updateLayerList();
    // prekresleni projektu
    this->project->requestRepaint();
}

void LayerManager::on_button_down_clicked()
{
    if(this->project == NULL) {
        QMessageBox::warning(
                    this,
                    tr("Move layer down"),
                    DIALOG_PROJECT_NOT_EXISTS);
        return;
    }

    Layer *l = this->project->getSelectedLayer();
    if(l == NULL) {
        QMessageBox::warning(
                    this,
                    tr("Move layer down"),
                    DIALOG_NO_LAYER);
        return;
    }

    // presun o jedno dolu
    this->project->moveSelectedLayerDown();
    // update layer listu
    this->updateLayerList();
    // prekresleni projektu
    this->project->requestRepaint();
}

void LayerManager::on_listWidget_itemSelectionChanged()
{
    if(this->project == NULL || this->listWidget == NULL) return;

    // ziskani vybraneho itemu z list widgetu
    QList<QListWidgetItem*> items = this->listWidget->selectedItems();
    if(items.isEmpty()) return;
    QListWidgetItem *item = items.first();

    // pomoci itemu najde odpovidajici widget vrstvy
    QWidget *widget = this->listWidget->itemWidget(item);
    if(widget == NULL) return;

    // pres widget pristoupi k jeho vrstve
    // v projektu nastavi nove vybranou vrstvu
    Layer *l = ((LayerWidget*)widget)->getLayer();
    this->project->setSelectedLayer(l);
}

