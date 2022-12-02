#include "project.h"


#include <QPainter>
#include <QtMath>
#include <QFile>


Project::Project(const QString &name, const QString &path, const QSize &size)
{
    this->setName(name);
    this->setPath(path);
    this->setSize(size);
    this->selected_layer = NULL;
}

Project::~Project()
{
    for(Layer *l : this->layers) {
        if(l) delete l;
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

const Layers_t &Project::getLayers() const {
    return this->layers;
}

bool Project::addLayer(Layer *layer)
{
    if(layer == NULL) return false;

    this->layers.push_back(layer);

    return true;
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

void Project::paintEvent(QPainter &painter, const QPoint &offset) {
    // vykresleni pozadi obrazku (sachovnice)
    painter.fillRect(
                offset.x(),
                offset.y(),
                this->size.width(),
                this->size.height(),
                QBrush(Qt::white, Qt::SolidPattern));

    QBrush brush(QColor(200, 200, 200), Qt::SolidPattern);
    int step = qMax(this->size.width(), this->size.height()) / 40;
    int step2 = 2 * step;
    int y_end = offset.y() + this->size.height();
    int x_end = offset.x() + this->size.width();

    int x, x_offset, step_x, step_y;
    for(int y = offset.y(), i = 0; y < y_end; y += step) {
        x_offset = i++ % 2 == 0 ? 0.0f : step;
        step_y = y + step < y_end ? step : (y_end - y);
        for(x = offset.x() + x_offset; x < x_end; x += step2) {
            step_x = x + step < x_end ? step : (x_end - x);
            painter.fillRect(x, y, step_x, step_y, brush);
        }
    }

    // vykresli vrstvy projektu
    for(Layer *layer : this->layers) {
        if(layer) {
            layer->paintEvent(painter, offset);
        }
    }

    // outline
    painter.setPen(Qt::black);
    painter.drawRect(offset.x(), offset.y(), this->size.width(), this->size.height());
}

void Project::exportEvent(QPainter &painter) {
    // vykresli vrstvy
    QPoint offset(0, 0);
    for(Layer *layer : this->layers) {
        if(layer) {
            layer->paintEvent(painter, offset);
        }
    }
}

