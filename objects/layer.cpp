#include "layer.h"

Layer::Layer(QObject *parent, const QString &name) : QObject(parent)
{
    this->name = name;
    this->visibility = true;
    this->opacity = 1.0;
}

Layer::~Layer() {

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

float Layer::getOpacity() const
{
    return this->opacity;
}

void Layer::setOpacity(float newOpacity)
{
    this->opacity = newOpacity;
}
