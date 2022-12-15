#include "layer.h"

#include "project.h"

#include <QRegion>


Layer::Layer(QObject *parent, const QString &name) : QObject(parent)
{
    this->name = name;
    this->visibility = true;
    this->opacity = 1.0;
    this->antialiasing = true;
    this->blendMode = NORMAL;
    this->mask = NULL;
}

Layer::~Layer() {
    if(this->mask) delete this->mask;
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
    this->opacity = qMax(0.0, qMin(1.0, (float) newOpacity));
}

void Layer::requestRepaint()
{
    QObject *parent = this->parent();
    if(parent) {
        ((Project*)parent)->requestRepaint();
    }
}

QSize Layer::getSize() const
{
    QObject *parent = this->parent();
    if(parent) {
        return ((Project*)parent)->getSize();
    }
    return QSize(0, 0);
}

void Layer::enableAntialiasing(bool enabled)
{
    this->antialiasing = enabled;
}

bool Layer::isAntialiasingEnabled() const
{
    return this->antialiasing;
}

LayerBlendMode Layer::getBlendMode() const
{
    return this->blendMode;
}

void Layer::setBlendMode(LayerBlendMode newBlendMode)
{
    this->blendMode = newBlendMode;
}

void Layer::applyLayerMask(QPainter &painter)
{
    if(this->mask != NULL) {
        painter.setClipRegion(QRegion(*this->mask));
    }
}

void Layer::createMask(const QSize size)
{
    if(this->mask) delete this->mask;
    this->mask = new QBitmap(size);
}

void Layer::resizeMask(const QSize size)
{
    if(this->mask == NULL) {
        this->createMask(size);
    } else {
        //this->mask = this->mask->scaled(size.width(), size.height(), Qt::KeepAspectRatio);
    }
}

QBitmap *Layer::getMask() const
{
    return this->mask;
}

void Layer_paintBgGrid(QPainter &painter, const QPoint &offset, const QSize &viewPort,
                       const QSize &size, const float step)
{
    // vykresleni pozadi obrazku (sachovnice)
    painter.fillRect(
                offset.x(),
                offset.y(),
                size.width(),
                size.height(),
                QBrush(Qt::white, Qt::SolidPattern));

    QBrush brush(QColor(200, 200, 200), Qt::SolidPattern);
    float step2 = 2 * step;

    // vypocet zacatku a konce vykreslovani sachovanice v zavislosti na offsetu a velikosti viewportu
    int x_start = offset.x();
    if(x_start < 0) {
        x_start = -((-x_start) % (int)(step2));
    }
    int y_start = offset.y();
    if(y_start < 0) {
        y_start = -((-y_start) % (int)(step2));
    }
    int x_end = qMin(offset.x() + size.width(), viewPort.width());
    int y_end = qMin(offset.y() + size.height(), viewPort.height());

    // vykresleni sachovanice
    float x, y = y_start;
    float x_offset, step_x, step_y;
    for(int i = 0; (int)y < y_end; y += step) {
        x_offset = i++ % 2 == 0 ? 0.0f : step;
        step_y = y + (int)step < y_end ? step : (y_end - y);
        for(x = x_offset + x_start; (int)x < x_end; x += step2) {
            step_x = x + (int)step < x_end ? step : (x_end - x);
            painter.fillRect(x, y, step_x, step_y, brush);
        }
    }
}
