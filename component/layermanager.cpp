#include "layermanager.h"


LayerManager::LayerManager(QWidget *parent) : QWidget(parent)
{
    this->setMaximumWidth(500);

    this->mainLayout = new QVBoxLayout(this);

    // list s vrstvama
    this->listWidget = new QListWidget(this);
    this->mainLayout->addWidget(this->listWidget);

    // tlaciky (pridani a odebrani vrstvy)
    this->buttons = new QWidget(this);
    this->buttonsLayout = new QHBoxLayout(this->buttons);

    // add layer
    this->button_addLayer = new QPushButton();
    this->button_addLayer->setToolTip(QString("Add layer"));
    this->button_addLayer->setIcon(QIcon(":/src/icons/new_layer.png"));
    this->button_addLayer->setIconSize(QSize(22, 22));
    this->buttonsLayout->addWidget(this->button_addLayer);

    // remove layer
    this->button_removeLayer = new QPushButton();
    this->button_removeLayer->setToolTip(QString("Remove layer"));
    this->button_removeLayer->setIcon(QIcon(":/src/icons/remove_layer.png"));
    this->button_removeLayer->setIconSize(QSize(22, 22));
    this->buttonsLayout->addWidget(this->button_removeLayer);

    this->mainLayout->addWidget(this->buttons);

    // test ...
    this->listWidget->insertItem(0, new QListWidgetItem(tr("Layer 1"), this->listWidget));
    this->listWidget->insertItem(0, new QListWidgetItem(tr("Layer 2"), this->listWidget));
    this->listWidget->insertItem(0, new QListWidgetItem(tr("Layer 3"), this->listWidget));
    this->listWidget->insertItem(0, new QListWidgetItem(tr("Layer 4"), this->listWidget));
}

LayerManager::~LayerManager() {
    if(buttonsLayout) delete buttonsLayout;
    if(mainLayout) delete mainLayout;
    if(buttons) delete buttons;
    if(listWidget) delete listWidget;
    if(button_addLayer) delete button_addLayer;
    if(button_removeLayer) delete button_removeLayer;
}
