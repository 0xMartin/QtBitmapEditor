#include "project.h"

Project::Project(const QString &name, const QSize &size)
{
    this->name = name;
    this->size = size;
}

const QString &Project::getName() const
{
    return this->name;
}

bool Project::setName(const QString &name) {
    if(name.length() < 3) return false;
    this->name = name;
    return true;
}

bool Project::setSize(const QSize &size) {
    if(size.width() < 1 || size.height() < 1) return false;
    this->size = size;
    return true;
}

QSize &Project::getSize() {
    return this->size;
}

const Layers_t &Project::getLayers() const {
    return this->layers;
}

void Project::paintEvent(QPaintEvent *event) {
    // outline
    // vykresli vrstvy
    for(Layer &layer : this->layers) {
        layer.paintEvent(event);
    }
}

void Project::exportEvent(QPaintEvent *event) {
    // vykresli vrstvy
    for(Layer &layer : this->layers) {
        layer.paintEvent(event);
    }
}
