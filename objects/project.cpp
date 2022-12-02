#include "project.h"

#include <QDir>
#include <QPainter>

Project::Project(const QString &name, const QString &path, const QSize &size)
{
    this->setName(name);
    this->setPath(path);
    this->setSize(size);
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

const Layers_t &Project::getLayers() const {
    return this->layers;
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

void Project::paintEvent(QPainter &painter, const QPoint &offset) {
    // outline
    painter.setPen(Qt::red);
    painter.drawRect(offset.x(), offset.y(), this->size.width(), this->size.height());

    // vykresli vrstvy
    for(Layer &layer : this->layers) {
        layer.paintEvent(painter, offset);
    }
}

void Project::exportEvent(QPainter &painter) {
    // vykresli vrstvy
    QPoint offset(0, 0);
    for(Layer &layer : this->layers) {
        layer.paintEvent(painter, offset);
    }
}
