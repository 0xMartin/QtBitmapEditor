#include "project.h"

#include <QWidget>
#include <QPainter>
#include <QtMath>
#include <QFile>


Project::Project(QObject *parent, const QString &name, const QString &path, const QSize &size) : QObject(parent)
{
    this->setName(name);
    this->setPath(path);
    this->setSize(size);
    this->selected_layer = NULL;
    this->layers = new Layers_t();
}

Project::~Project()
{
    if(this->layers) {
        for(Layer *l : *this->layers) {
            if(l) delete l;
        }
        delete this->layers;
    }
}

const QString &Project::getName() const
{
    return this->name;
}

bool Project::setName(const QString &name) {
    if(name.length() < PROJECT_MIN_NAME_LENGTH) return false;
    this->name = name;
    return true;
}

const QString &Project::getPath() const
{
    return this->path;
}

void Project::setPath(const QString &path)
{
    this->path = path;
}

bool Project::setSize(const QSize &size) {
    if(size.width() < 1 || size.height() < 1) return false;
    this->size = size;
    return true;
}

QSize &Project::getSize() {
    return this->size;
}

Layers_t *Project::getLayers() const {
    return this->layers;
}

bool Project::addLayer(Layer *layer)
{
    if(layer == NULL) return false;
    if(this->layers == NULL) return false;

    this->layers->push_back(layer);

    return true;
}

bool Project::removeLayer(Layer *layer)
{
    if(layer == NULL) return false;
    if(this->layers == NULL) return false;

    qsizetype index = 0;
    for(Layer *l : *this->layers) {
        if(l == layer) {
            // odstrani vrstvu (delete + odebrani z listu)
            delete (this->layers->takeAt(index));
            // pokud odebrana vrstava byla v projektu oznacena tak zrusi oznaceni
            if(layer == this->selected_layer)
                this->selected_layer = NULL;
            return true;
        }
        ++index;
    }

    return false;
}

bool Project::saveProject() const
{
    if(path.length() == 0) return false;

    QFile file(path);
    if(!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    file.close();

    return true;
}

Layer *Project::getSelectedLayer() const
{
    return this->selected_layer;
}

void Project::setSelectedLayer(Layer *newSelected_layer)
{
    this->selected_layer = newSelected_layer;
}

void Project::requestRepaint()
{
    QObject *parent = this->parent();
    if(parent) {
        ((QWidget*) parent)->repaint();
    }
}

bool Project::moveSelectedLayerUp()
{
    if(this->selected_layer == NULL) return false;
    qsizetype index = 0;
    for(Layer *l : *this->layers) {
        if(l == this->selected_layer) {
            if(index + 1 < this->layers->size()) {
                Layer *layer = this->layers->takeAt(index);
                this->layers->insert(index + 1, layer);
                return true;
            }
        }
        ++index;
    }
    return false;
}

bool Project::moveSelectedLayerDown()
{
    if(this->selected_layer == NULL) return false;
    qsizetype index = 0;
    for(Layer *l : *this->layers) {
        if(l == this->selected_layer) {
            if(index > 0) {
                Layer *layer = this->layers->takeAt(index);
                this->layers->insert(index - 1, layer);
                return true;
            }
        }
        ++index;
    }
    return true;
}

void Project::paintEvent(QPainter &painter) {
    // vykresleni pozadi obrazku (sachovnice)
    Layer_paintBgGrid(painter, this->size, 16);

    // vykresli vrstvy projektu
    if(this->layers) {
        for(Layer *layer : *this->layers) {
            if(layer) {
                if(!layer->isVisible()) continue;
                layer->paintEvent(painter);
            }
        }
    }

    // outline
    painter.setPen(Qt::black);
    painter.drawRect(0, 0, this->size.width(), this->size.height());
}

void Project::exportEvent(QPainter &painter) {
    // vykresli vrstvy
    QPoint offset(0, 0);
    if(this->layers) {
        for(Layer *layer : *this->layers) {
            if(layer) {
                layer->paintEvent(painter);
            }
        }
    }
}

