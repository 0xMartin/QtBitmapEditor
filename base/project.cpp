#include "project.h"

#include "config.h"

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
    if(this->layers->size() <= 1) return false;

    qsizetype index = 0;
    for(Layer *l : *this->layers) {
        if(l == layer) {
            // pokud odebrana vrstava byla v projektu oznacena tak zrusi oznaceni
            if(layer == this->selected_layer)
                this->selected_layer = NULL;
            // odstrani vrstvu (delete + odebrani z listu)
            delete (this->layers->takeAt(index));
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

bool Project::mergaSeletedLayerDown()
{
    if(this->selected_layer == NULL) return false;
    if(this->layers->length() < 2) return false;
    qsizetype index = 0;
    for(Layer *l : *this->layers) {
        if(l == this->selected_layer) {
            if(index > 0) {
                Layer *buttom = this->layers->at(index - 1);
                Layer *top = this->layers->takeAt(index);
                //########################################################################
                //merge TODO
                //########################################################################
                return true;
            }
        }
        ++index;
    }
    return true;
}

bool Project::dupliceteLayer()
{
    if(this->selected_layer == NULL) return false;
    qsizetype index = 0;
    for(Layer *l : *this->layers) {
        if(l == this->selected_layer) {
            Layer *duplicate = this->selected_layer->createDuplicate();
            if(duplicate == NULL) {
                return false;
            }
            this->layers->insert(index + 1, duplicate);
            return true;
        }
        ++index;
    }
    return true;
}

void Project::paintEvent(QPainter &painter) {
    // vykresli vrstvy projektu
    if(this->layers) {
        for(Layer *layer : *this->layers) {
            if(layer) {
                if(!layer->isVisible()) continue;
                // antialiasing
                painter.setRenderHint(QPainter::Antialiasing, layer->isAntialiasingEnabled());
                // opacity
                painter.setOpacity(layer->getOpacity());
                // blend mode
                switch (layer->getBlendMode()) {
                case NORMAL:
                    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
                    break;
                case ADDITION:
                    painter.setCompositionMode(QPainter::CompositionMode_Plus);
                    break;
                case MULTIPLY:
                    painter.setCompositionMode(QPainter::CompositionMode_Multiply);
                    break;
                case SCREEN:
                    painter.setCompositionMode(QPainter::CompositionMode_Screen);
                    break;
                case OVERLAY:
                    painter.setCompositionMode(QPainter::CompositionMode_Overlay);
                    break;
                case DARKEN:
                    painter.setCompositionMode(QPainter::CompositionMode_Darken);
                    break;
                case LIGHTEN:
                    painter.setCompositionMode(QPainter::CompositionMode_Lighten);
                    break;
                case COLOR_DODGE:
                    painter.setCompositionMode(QPainter::CompositionMode_ColorDodge);
                    break;
                case COLOR_BURN:
                    painter.setCompositionMode(QPainter::CompositionMode_ColorBurn);
                    break;
                case HARD_LIGHT:
                    painter.setCompositionMode(QPainter::CompositionMode_HardLight);
                    break;
                case SOFT_LIGHT:
                    painter.setCompositionMode(QPainter::CompositionMode_SoftLight);
                    break;
                case DIFFERENCE:
                    painter.setCompositionMode(QPainter::CompositionMode_Difference);
                    break;
                case EXCLUSION:
                    painter.setCompositionMode(QPainter::CompositionMode_Exclusion);
                    break;
                }
                // paint layer
                layer->paintEvent(painter);
            }
        }
    }

    // reset blend mode
    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
}
