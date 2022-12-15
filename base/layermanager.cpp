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
    this->image->setStyleSheet("border-width: 2px;");
    this->repaintLayer();
    this->hBoxLayout->addWidget(this->image);

    // nahled masky vrstvy
    this->mask = new QLabel(this);
    this->mask->setStyleSheet("border-width: 2px;");
    this->repaintMask();
    this->hBoxLayout->addWidget(this->mask);

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
    // vypocet sirky a vysky nahledu vrstvy (maxilani sirka je 110)
    int c_w = height * ((float)layer->getSize().width() / layer->getSize().height());
    int c_h = height;
    if(c_w > 110) {
        c_w = 110;
        c_h = 110 * ((float)layer->getSize().height() / layer->getSize().width());
    }

    // priprava pixmapy
    QPixmap pixmap(c_w, c_h);
    this->image->setFixedSize(QSize(c_w, c_h));
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(0, 0, pixmap.width(), pixmap.height(), Qt::black);
    Layer_paintBgGrid(painter, QPoint(0, 0), pixmap.size(), QSize(pixmap.width(), pixmap.height()), 6);

    // vypocet scale tak aby se do pixelmapy vlezla nejdelsi strana
    float scale = (float)pixmap.width() / qMax(c_w, layer->getSize().width());
    // pokud je samotna vrstva mensi nez minimalni velikost nahledu (height x height) -> musi zvetsit
    if(layer->getSize().width() < height && layer->getSize().height()) {
        scale *= (float)height / qMax(layer->getSize().width(), layer->getSize().height());
    }
    painter.scale(scale, scale);

    // vykresli vrstvu do nahledu
    layer->paintEvent(painter);
    painter.end();

    // labelu nastavi pixmapu
    this->image->setPixmap(pixmap);
}

void LayerWidget::repaintMask()
{
    if(this->layer->getMask() == NULL) {
        this->mask->setVisible(false);
        return;
    } else {
        this->mask->setVisible(true);
    }

    // vypocet sirky a vysky nahledu vrstvy (maxilani sirka je 110)
    int c_w = height * ((float)layer->getSize().width() / layer->getSize().height());
    int c_h = height;
    if(c_w > 110) {
        c_w = 110;
        c_h = 110 * ((float)layer->getSize().height() / layer->getSize().width());
    }

    // priprava pixmapy
    QPixmap pixmap(c_w, c_h);
    this->mask->setFixedSize(QSize(c_w, c_h));
    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.fillRect(0, 0, pixmap.width(), pixmap.height(), Qt::white);

    // vypocet scale tak aby se do pixelmapy vlezla nejdelsi strana
    float scale = (float)pixmap.width() / qMax(c_w, layer->getSize().width());
    // pokud je samotna vrstva mensi nez minimalni velikost nahledu (height x height) -> musi zvetsit
    if(layer->getSize().width() < height && layer->getSize().height()) {
        scale *= (float)height / qMax(layer->getSize().width(), layer->getSize().height());
    }
    painter.scale(scale, scale);

    // vykresli vrstvu do nahledu
    painter.setPen(Qt::black);
    painter.drawPixmap(0, 0, *this->layer->getMask());

    // vykresli preskrtnuty masky pokud neni aktivni
    if(!this->layer->isMaskActive()) {
        float inv = 1.0 / scale;
        painter.setPen(QPen(Qt::red, (int)(3 * inv)));
        painter.drawLine(0, 0, c_w * inv, c_h * inv);
        painter.drawLine(0, c_h * inv, c_w * inv, 0);
    }

    painter.end();

    // labelu nastavi pixmapu
    this->mask->setPixmap(pixmap);
}

void LayerWidget::setName(const QString &name)
{
    if(this->label == NULL) return;
    this->label->setText(name);
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
    this->mainLayout = new QVBoxLayout(this);
    this->mainLayout->setSpacing(0);
    this->setLayout(this->mainLayout);
    this->setMaximumWidth(1000);


    // header
    //------------------------------------------------------------------------------------------
    this->header = new QLabel(this);
    this->header->setText(tr("<b>LAYERS</b>"));
    this->header->setStyleSheet("background: rgb(43, 43, 43); color: rgb(224, 224, 224); padding: 8px; border-bottom: 1px solid black; border-radius: 4px;");
    this->mainLayout->addWidget(this->header);


    // ovladni projektu (mode -> normal / prace s maskama)
    //------------------------------------------------------------------------------------------
    this->projectControl = new QWidget(this);
    this->mainLayout->addWidget(this->projectControl);
    this->projectControllLayout = new QHBoxLayout(this->projectControl);
    this->projectControl->setLayout(this->projectControllLayout);

    // project edit mode
    this->comboBox_editMode = new QComboBox(this);
    connect(this->comboBox_editMode, SIGNAL(currentIndexChanged(int)),
            this, SLOT(on_project_edit_mode_changed(int)));
    this->comboBox_editMode->setToolTip(tr("EDIT MODE"));
    this->comboBox_editMode->addItem(tr("IMAGE"));
    this->comboBox_editMode->addItem(tr("MASK"));
    this->projectControllLayout->addWidget(this->comboBox_editMode);


    // ovladani aktualne vybrane vrstvy
    //------------------------------------------------------------------------------------------
    this->layerControl = new QWidget(this);
    this->mainLayout->addWidget(this->layerControl);
    this->layerControllLayout = new QHBoxLayout(this->layerControl);
    this->layerControl->setLayout(this->layerControllLayout);

    // smooth
    this->checkBox_antialiasing = new QCheckBox(this);
    connect(this->checkBox_antialiasing, SIGNAL(toggled(bool)),
            this, SLOT(on_layer_antialiasing_toggled(bool)));
    this->checkBox_antialiasing->setText(tr("Smooth"));
    this->checkBox_antialiasing->setToolTip(tr("Smooth Painting Mode"));
    this->layerControllLayout->addWidget(this->checkBox_antialiasing);

    // blend mode
    this->comboBox_blend = new QComboBox(this);
    connect(this->comboBox_blend, SIGNAL(currentIndexChanged(int)),
            this, SLOT(on_layer_blend_changed(int)));
    this->comboBox_blend->setToolTip(tr("BLEND MODE"));
    this->comboBox_blend->addItem(tr("NORMAL"));
    this->comboBox_blend->addItem(tr("ADDITION"));
    this->comboBox_blend->addItem(tr("MULTIPLY"));
    this->comboBox_blend->addItem(tr("SCREEN"));
    this->comboBox_blend->addItem(tr("OVERLAY"));
    this->comboBox_blend->addItem(tr("DARKEN"));
    this->comboBox_blend->addItem(tr("LIGHTEN"));
    this->comboBox_blend->addItem(tr("COLOR DODGE"));
    this->comboBox_blend->addItem(tr("COLOR BURN"));
    this->comboBox_blend->addItem(tr("HARD LIGHT"));
    this->comboBox_blend->addItem(tr("SOFT LIGHT"));
    this->comboBox_blend->addItem(tr("DIFFERENCE"));
    this->comboBox_blend->addItem(tr("EXCLUSION"));
    this->layerControllLayout->addWidget(this->comboBox_blend);

    // opacity
    this->spinbox_opacity = new QSpinBox(this);
    connect(this->spinbox_opacity, SIGNAL(valueChanged(int)),
            this, SLOT(on_layer_opacity_changed(int)));
    this->spinbox_opacity->setToolTip(QString(tr("Opacity")));
    this->spinbox_opacity->setPrefix("Opacity: ");
    this->spinbox_opacity->setSuffix("%");
    this->spinbox_opacity->setMaximum(100);
    this->spinbox_opacity->setMinimum(0);
    this->spinbox_opacity->setValue(100);
    this->layerControllLayout->addWidget(this->spinbox_opacity);

    this->updateLayerControllBinding();

    // list s vrstvama
    //------------------------------------------------------------------------------------------
    this->listWidget = new QListWidget(this);
    connect(this->listWidget, SIGNAL(itemSelectionChanged()),
            this, SLOT(on_listWidget_itemSelectionChanged()));
    connect(this->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
            this, SLOT(on_listWidget_itemDoubleClicked(QListWidgetItem*)));
    this->listWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this->listWidget, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showContextMenu(QPoint)));
    this->mainLayout->addWidget(this->listWidget);


    // tlaciky pro ovladani listu vrstev (add, remove, up, down, vytvoreni masky, odebrani masky)
    //------------------------------------------------------------------------------------------
    this->listControl = new QWidget(this);
    this->mainLayout->addWidget(this->listControl);
    this->listControllLayout = new QHBoxLayout(this->listControl);
    this->listControl->setLayout(this->listControllLayout);

    //spacer
    this->listControllLayout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));

    // add layer tlacitko
    this->button_addLayer = new QPushButton(this->listControl);
    connect(this->button_addLayer, SIGNAL(clicked()), this, SLOT(on_button_addLayer_clicked()));
    this->button_addLayer->setToolTip(QString(tr("Add Layer")));
    this->button_addLayer->setIcon(QIcon(":/src/icons/new_layer.png"));
    this->listControllLayout->addWidget(this->button_addLayer);

    // remove  tlacitko
    this->button_removeLayer = new QPushButton(this->listControl);
    connect(this->button_removeLayer, SIGNAL(clicked()), this, SLOT(on_button_removeLayer_clicked()));
    this->button_removeLayer->setToolTip(QString(tr("Remove Layer")));
    this->button_removeLayer->setIcon(QIcon(":/src/icons/remove_layer.png"));
    this->listControllLayout->addWidget(this->button_removeLayer);

    // up tlacitko
    this->button_up = new QPushButton(this->listControl);
    connect(this->button_up, SIGNAL(clicked()), this, SLOT(on_button_up_clicked()));
    this->button_up->setToolTip(QString(tr("Move Layer Up")));
    this->button_up->setIcon(QIcon(":/src/icons/arrow_up.png"));
    this->listControllLayout->addWidget(this->button_up);

    // down tlacitko
    this->button_down = new QPushButton(this->listControl);
    connect(this->button_down, SIGNAL(clicked()), this, SLOT(on_button_down_clicked()));
    this->button_down->setToolTip(QString(tr("Move Layer Down")));
    this->button_down->setIcon(QIcon(":/src/icons/arrow_down.png"));
    this->listControllLayout->addWidget(this->button_down);

    // mask tlacitko
    this->button_mask = new QPushButton(this->listControl);
    connect(this->button_mask, SIGNAL(clicked()), this, SLOT(on_button_mask_clicked()));
    this->button_mask->setToolTip(QString(tr("Layer Mask")));
    this->button_mask->setIcon(QIcon(":/src/icons/layer_mask.png"));
    this->listControllLayout->addWidget(this->button_mask);
}

LayerManager::~LayerManager() {
    if(this->header) delete this->header;
    if(this->mainLayout) delete this->mainLayout;
    if(this->listControl) delete this->listControl;
    if(this->layerControl) delete this->layerControl;
    if(this->projectControl) delete this->projectControl;
    if(this->listWidget) delete this->listWidget;
}

void LayerManager::setProject(Project *project)
{
    this->project = project;
    this->updateLayerList();
    connect(this->project, SIGNAL(repaintSignal(Layer*)), this, SLOT(on_project_repaintSignal(Layer*)));
    this->updateLayerControllBinding();
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
        LayerWidget *w = new LayerWidget(*it, 50); // dealokace pri clear
        QListWidgetItem *item = new QListWidgetItem(listWidget); // dealokace pri clear
        item->setSizeHint(QSize(80, 70));
        this->listWidget->insertItem(0, item);
        this->listWidget->setItemWidget(item, w);
        // oznaceni teto vrstvy ?
        if(w->getLayer() == this->project->getSelectedLayer()) {
            this->listWidget->setCurrentItem(item);
        }
    }
}

void LayerManager::selectMode(ProjectEditMode_t mode)
{
    switch (mode) {
    case PROJECT_EDIT:
        this->comboBox_editMode->setCurrentIndex(0);
        break;
    case MASK_EDIT:
        this->comboBox_editMode->setCurrentIndex(1);
        break;
    }
}

void LayerManager::changeEvent(QEvent * event)
{
    // vzdy fixni velikost
    if(this->button_addLayer) this->button_addLayer->setFixedSize(QSize(30, 30));
    if(this->button_removeLayer) this->button_removeLayer->setFixedSize(QSize(30, 30));
    if(this->button_up) this->button_up->setFixedSize(QSize(30, 30));
    if(this->button_down) this->button_down->setFixedSize(QSize(30, 30));
    if(this->button_mask) this->button_mask->setFixedSize(QSize(30, 30));
}

void LayerManager::updateLayerControllBinding()
{
    if(this->project == NULL) {
        this->layerControl->setEnabled(false);
        return;
    }
    Layer *l = this->project->getSelectedLayer();
    if(l == NULL) {
        this->layerControl->setEnabled(false);
    } else {
        this->layerControl->setEnabled(true);
        this->spinbox_opacity->setValue(l->getOpacity() * 100.0);
        this->checkBox_antialiasing->setChecked(l->isAntialiasingEnabled());
        // ciselna hodnota LayerBlendMode presne odpovida poradi v comboboxu
        this->comboBox_blend->setCurrentIndex(l->getBlendMode());
    }
}

void LayerManager::on_project_repaintSignal(Layer *layer)
{
    if(layer == NULL) return;
    LayerWidget *widget;
    for(int i = 0; i < this->listWidget->count(); ++i) {
        widget = (LayerWidget*)this->listWidget->itemWidget(this->listWidget->item(i));
        if(widget == NULL) continue;
        if(widget->getLayer() == layer) {
            widget->repaintLayer();
            widget->repaintMask();
            widget->repaint();
            break;
        }
    }
}

/*********************************************************************************************************************/
// LayerManager - EVENTY PRO OVLADACI PRVKY
/*********************************************************************************************************************/

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
        this->project->insertLayerAbove(layer);
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

    if(this->project->getLayers()->size() <= 1) {
        QMessageBox::warning(
                    this,
                    tr("Remove layer"),
                    tr("It is not possible to remove all layers"));
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
        // odebere vybranou vrstvu a pak oznacni vrstvu pod touto odebranou vrstvou
        qsizetype i = this->project->getLayers()->indexOf(l) - 1;
        this->project->removeLayer(l);
        this->project->setSelectedLayer(this->project->getLayers()->at(qMax(i, 0)));
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

void LayerManager::on_button_mask_clicked() {
    if(this->project == NULL) {
        QMessageBox::warning(
                    this,
                    tr("Mask Add/Remove"),
                    DIALOG_PROJECT_NOT_EXISTS);
        return;
    }

    Layer *l = this->project->getSelectedLayer();
    if(l == NULL) {
        QMessageBox::warning(
                    this,
                    tr("Mask Add/Remove"),
                    DIALOG_NO_LAYER);
        return;
    }

    if(l->getMask() == NULL) {
        // vytvori novou masku
        l->createMask();
    } else {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(
                    this,
                    tr("Remove Mask"),
                    QString(tr("Do you want to delete the mask for layer named [%1]?")).arg(l->getName()),
                    QMessageBox::Yes|QMessageBox::No);
        if(reply == QMessageBox::Yes) {
            // odstrani masku
            l->deleteMask();
        }
    }

    // proveda zmeni v listu (zobrazi/skryje masku)
    LayerWidget *widget;
    for(int i = 0; i < this->listWidget->count(); ++i) {
        widget = (LayerWidget*)this->listWidget->itemWidget(this->listWidget->item(i));
        if(widget == NULL) continue;
        if(widget->getLayer() == l) {
            widget->repaintMask();
            break;
        }
    }

    // prekresli projekt
    this->project->requestRepaint();
}

void LayerManager::on_layer_merge_down()
{
    if(this->project == NULL) {
        QMessageBox::warning(
                    this,
                    tr("Merge layer down"),
                    DIALOG_PROJECT_NOT_EXISTS);
        return;
    }

    Layer *l = this->project->getSelectedLayer();
    if(l == NULL) {
        QMessageBox::warning(
                    this,
                    tr("Merge layer down"),
                    DIALOG_NO_LAYER);
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
                this,
                tr("Merge layer down"),
                QString(tr("Do you want to merage down the layer named [%1]?")).arg(l->getName()),
                QMessageBox::Yes|QMessageBox::No);
    if(reply == QMessageBox::Yes) {
        if(this->project->mergaSeletedLayerDown()) {
            this->updateLayerList();
            this->project->requestRepaint();
        } else {
            QMessageBox::warning(
                        this,
                        tr("Move layer down"),
                        tr("Failed to merge layers"));
        }
    }
}

void LayerManager::on_layer_duplicate()
{
    if(this->project == NULL) {
        QMessageBox::warning(
                    this,
                    tr("Duplicate layer"),
                    DIALOG_PROJECT_NOT_EXISTS);
        return;
    }

    Layer *l = this->project->getSelectedLayer();
    if(l == NULL) {
        QMessageBox::warning(
                    this,
                    tr("Duplicate layer"),
                    DIALOG_NO_LAYER);
        return;
    }

    if(this->project->dupliceteLayer()) {
        this->updateLayerList();
        this->project->requestRepaint();
    } else {
        QMessageBox::warning(
                    this,
                    tr("Duplicate layer"),
                    tr("Failed to duplicate layer"));
    }
}

void LayerManager::on_mask_active_deactivate()
{
    if(this->project == NULL) {
        QMessageBox::warning(
                    this,
                    tr("Mask Activate/Deactivate"),
                    DIALOG_PROJECT_NOT_EXISTS);
        return;
    }

    Layer *l = this->project->getSelectedLayer();
    if(l == NULL) {
        QMessageBox::warning(
                    this,
                    tr("Mask Activate/Deactivate"),
                    DIALOG_NO_LAYER);
        return;
    }

    // aktivuje / deaktivuje masku vybrane vrstvy
    l->setMaskActive(!l->isMaskActive());

    // prekresli projekt
    this->project->requestRepaint();

    // proveda zmeni v listu (prekresleni masky widgetu vybrane vrstvy)
    LayerWidget *widget;
    for(int i = 0; i < this->listWidget->count(); ++i) {
        widget = (LayerWidget*)this->listWidget->itemWidget(this->listWidget->item(i));
        if(widget == NULL) continue;
        if(widget->getLayer() == l) {
            widget->repaintMask();
            break;
        }
    }
}

void LayerManager::on_layer_rename()
{
    if(this->project == NULL) {
        QMessageBox::warning(
                    this,
                    tr("Rename layer"),
                    DIALOG_PROJECT_NOT_EXISTS);
        return;
    }

    Layer *l = this->project->getSelectedLayer();
    if(l == NULL) {
        QMessageBox::warning(
                    this,
                    tr("Rename layer"),
                    DIALOG_NO_LAYER);
        return;
    }

    bool ok;
    QString name = QInputDialog::getText(
                this,
                tr("Rename layer"),
                tr("Layer name:"),
                QLineEdit::Normal,
                l->getName(),
                &ok);

    if (ok && !name.isEmpty()) {
        // nastavy jmeno
        l->setName(name);
        // proveda zmeni v listu
        LayerWidget *widget;
        for(int i = 0; i < this->listWidget->count(); ++i) {
            widget = (LayerWidget*)this->listWidget->itemWidget(this->listWidget->item(i));
            if(widget == NULL) continue;
            if(widget->getLayer() == l) {
                widget->setName(name);
                break;
            }
        }
    }
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

    // update bindingu pro ovladaci prvky vrstvy
    this->updateLayerControllBinding();

    // prekasli projekt (v nekterych jinych praconich modech jinich nez "PROJECT_EDIT" muze byt vyzadovano)
    this->project->requestRepaint();
}

void LayerManager::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    this->on_layer_rename();
}

void LayerManager::showContextMenu(const QPoint &pos)
{
    QMenu contextMenu(tr("Menu"), this->listWidget);

    QAction action1(tr("Rename Layer"), this->listWidget);
    connect(&action1, SIGNAL(triggered()), this, SLOT(on_layer_rename()));
    QAction action3(tr("Move Up"), this->listWidget);
    connect(&action3, SIGNAL(triggered()), this, SLOT(on_button_up_clicked()));
    QAction action4(tr("Move Down"), this->listWidget);
    connect(&action4, SIGNAL(triggered()), this, SLOT(on_button_down_clicked()));
    QAction action5(tr("Add Layer"), this->listWidget);
    connect(&action5, SIGNAL(triggered()), this, SLOT(on_button_addLayer_clicked()));
    QAction action6(tr("Remove Layer"), this->listWidget);
    connect(&action6, SIGNAL(triggered()), this, SLOT(on_button_removeLayer_clicked()));
    QAction action7(tr("Duplicate Layer"), this->listWidget);
    connect(&action7, SIGNAL(triggered()), this, SLOT(on_layer_duplicate()));
    QAction action8(tr("Merge Down"), this->listWidget);
    connect(&action8, SIGNAL(triggered()), this, SLOT(on_layer_merge_down()));
    QAction action9(tr("Add/Remove Mask"), this->listWidget);
    connect(&action9, SIGNAL(triggered()), this, SLOT(on_button_mask_clicked()));
    QAction action10(tr("Activate/Deactivate Mask"), this->listWidget);
    connect(&action10, SIGNAL(triggered()), this, SLOT(on_mask_active_deactivate()));

    contextMenu.addAction(&action1);
    contextMenu.addSeparator();
    contextMenu.addAction(&action3);
    contextMenu.addAction(&action4);
    contextMenu.addSeparator();
    contextMenu.addAction(&action5);
    contextMenu.addAction(&action6);
    contextMenu.addAction(&action7);
    contextMenu.addAction(&action8);
    contextMenu.addSeparator();
    contextMenu.addAction(&action9);
    contextMenu.addAction(&action10);

    contextMenu.exec(this->listWidget->mapToGlobal(pos));
}

void LayerManager::on_layer_antialiasing_toggled(bool value)
{
    if(this->project == NULL) return;
    Layer *l = this->project->getSelectedLayer();
    if(l == NULL) return;
    l->enableAntialiasing(value);
    l->requestRepaint();
}

void LayerManager::on_layer_blend_changed(int index)
{
    if(this->project == NULL) return;
    Layer *l = this->project->getSelectedLayer();
    if(l == NULL) return;
    // ciselna hodnota LayerBlendMode presne odpovida poradi v comboboxu
    l->setBlendMode((LayerBlendMode)index);
    l->requestRepaint();
}

void LayerManager::on_layer_opacity_changed(int value)
{
    if(this->project == NULL) return;
    Layer *l = this->project->getSelectedLayer();
    if(l == NULL) return;
    l->setOpacity(value / 100.0);
    l->requestRepaint();
}

void LayerManager::on_project_edit_mode_changed(int index)
{
    if(this->project == NULL) return;
    switch (index) {
    case 0:
        this->project->setMode(PROJECT_EDIT);
        break;
    case 1:
        this->project->setMode(MASK_EDIT);
        break;
    }
    this->project->requestRepaint();
}

