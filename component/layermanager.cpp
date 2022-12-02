#include "layermanager.h"

#include <QMessageBox>
#include <QInputDialog>

#include "../dialogs.h"
#include "../objects/bitmaplayer.h"


LayerManager::LayerManager(QWidget *parent) : QWidget(parent)
{
    this->project = NULL;
    this->setMaximumWidth(500);

    this->mainLayout = new QVBoxLayout(this);

    // list s vrstvama
    this->listView = new QListView(this);
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

    // test ...
    //this->listWidget->insertItem(0, new QListWidgetItem(tr("Layer 1"), this->listWidget));
}

LayerManager::~LayerManager() {
    if(buttonsLayout) delete buttonsLayout;
    if(mainLayout) delete mainLayout;
    if(buttons) delete buttons;
    if(listView) delete listView;
    if(button_addLayer) delete button_addLayer;
    if(button_removeLayer) delete button_removeLayer;
}

void LayerManager::setProject(Project *project)
{
    this->project = project;
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
                "Layer " + QString::number(this->project->getLayers().size() + 1),
                &ok);
    if (ok && !text.isEmpty()) {
        qDebug() << text;
        Layer *layer = new BitmapLayer(text, this->project->getSize());
        this->project->addLayer(layer);
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
