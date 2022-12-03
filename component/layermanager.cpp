#include "layermanager.h"

#include <QMessageBox>
#include <QInputDialog>

#include "../dialogs.h"
#include "../objects/bitmaplayer.h"


/**********************************************************************************************/
// LayerListModel
/**********************************************************************************************/

LayerListModel::LayerListModel(QObject *parent) : QAbstractListModel(parent)
{
    this->list = NULL;
}

QVariant LayerListModel::data(const QModelIndex &index, int role) const
{
    if(this->list == NULL)
        return QVariant();
    if (!index.isValid())
        return QVariant();
    if (index.row() >= this->list->size())
        return QVariant();

    if (role == Qt::DisplayRole)
        return QVariant(QString("row [%1]").arg((this->list->at(index.row()))->getName()));
    else
        return QVariant();
}

int LayerListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    if(this->list == NULL) return 0;
    return this->list->size();
}

void LayerListModel::setList(Layers_t *list)
{
    this->list = list;
}

Layer *LayerListModel::getLayerAt(size_t index)
{
    if(this->list == NULL) return NULL;
    if(index >= this->list->size()) return NULL;
    return this->list->at(index);
}

/**********************************************************************************************/
// LayerManager
/**********************************************************************************************/

LayerManager::LayerManager(QWidget *parent) : QWidget(parent)
{
    this->project = NULL;
    this->setMaximumWidth(500);

    this->mainLayout = new QVBoxLayout(this);

    // list s vrstvama
    this->listView = new QListView(this);
    this->listModel = new LayerListModel();
    this->listView->setResizeMode(QListView::Adjust);
    this->listView->setSelectionMode(QAbstractItemView::SingleSelection);
    this->listView->setModel(this->listModel);
    this->connect(this->listView, QAbstractItemView::clicked, this, LayerListModel::on_button_removeLayer_clicked);
    this->mainLayout->addWidget(this->listView);

    // tlaciky (pridani a odebrani vrstvy)
    this->buttons = new QWidget(this);
    this->buttonsLayout = new QHBoxLayout(this->buttons);

    // add layer tlacitko
    this->button_addLayer = new QPushButton();
    this->connect(this->button_addLayer, SIGNAL(clicked()), this, SLOT(on_button_addLayer_clicked()));
    this->button_addLayer->setToolTip(QString(tr("Add layer")));
    this->button_addLayer->setIcon(QIcon(":/src/icons/new_layer.png"));
    this->button_addLayer->setIconSize(QSize(22, 22));
    this->buttonsLayout->addWidget(this->button_addLayer);

    // remove  tlacitko
    this->button_removeLayer = new QPushButton();
    this->connect(this->button_removeLayer, SIGNAL(clicked()), this, SLOT(on_button_removeLayer_clicked()));
    this->button_removeLayer->setToolTip(QString(tr("Remove layer")));
    this->button_removeLayer->setIcon(QIcon(":/src/icons/remove_layer.png"));
    this->button_removeLayer->setIconSize(QSize(22, 22));
    this->buttonsLayout->addWidget(this->button_removeLayer);

    this->mainLayout->addWidget(this->buttons);
}

LayerManager::~LayerManager() {
    if(this->buttonsLayout) delete this->buttonsLayout;
    if(this->mainLayout) delete this->mainLayout;
    if(this->buttons) delete this->buttons;
    if(this->listView) delete this->listView;
    if(this->button_addLayer) delete this->button_addLayer;
    if(this->button_removeLayer) delete this->button_removeLayer;
    if(this->listModel) delete this->listModel;
}

void LayerManager::setProject(Project *project)
{
    this->project = project;
    if(this->listModel) {
        this->listModel->setList(this->project->getLayers());
    }
}

Project *LayerManager::getProject() const
{
    return this->project;
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
        // repaint listview
        if(this->listView) {
            this->listView->doItemsLayout();
            this->listView->repaint();
        }
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

    }
}

void LayerManager::on_listView_clicked(const QModelIndex &index)
{
    if(this->project == NULL || this->listView == NULL) return;
    int row = this->listView->currentIndex().row();
    Layer *l = this->listModel->getLayerAt(row);
    qDebug() << row << ": " << l;
    this->project->setSelectedLayer(l);
}
