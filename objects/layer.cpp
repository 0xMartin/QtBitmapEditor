#include "layer.h"

Layer::Layer(const QString &name)
{
    this->name = name;
    this->visibility = true;
}

void Layer::setVisible(bool visibility) {
    this->visibility = visibility;
}

bool Layer::isVisible() const {
    return this->visibility;
}

void Layer::setName(const QString &name) {
    this->name = name;
}

QString & Layer::getName() {
    return this->name;
}
