#include "layer.h"

#include "project.h"

#include <QRegion>


Layer::Layer(QObject *project, const QString &name) : QObject(project)
{
    this->name = name;
    this->visibility = true;
    this->opacity = 1.0;
    this->antialiasing = true;
    this->blendMode = NORMAL;
    this->mask = NULL;
    this->maskActive = false;
}

Layer::~Layer() {
    if(this->mask) delete this->mask;
}

qint32 Layer::getType() const
{
    return 0;
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

const QString & Layer::getName() const {
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

void Layer::createMask()
{
    if(this->mask) delete this->mask;
    Project *parent = (Project*) this->parent();
    if(parent == NULL) return;
    this->mask = new QBitmap(parent->getSize());
    if(this->mask != NULL) {
        this->mask->fill(Qt::black);
    }
    this->maskActive = true;
}

void Layer::updateMaskSize()
{
    if(this->mask == NULL) {
        this->createMask();
    } else {
        //this->mask = this->mask->scaled(size.width(), size.height(), Qt::KeepAspectRatio);
    }
}

void Layer::deleteMask()
{
    if(this->mask) delete this->mask;
    this->mask = NULL;
    this->maskActive = false;
}

void Layer::maskPaste(QBitmap *newMask)
{
    if(newMask == NULL) return;

    if(this->mask == NULL) {
        this->createMask();
    }

    if(this->mask != NULL) {
        if(this->mask->size().width() == newMask->size().width() &&
                this->mask->size().height() == newMask->size().height()) {
            *this->mask = *newMask;
        }
    }
}

QBitmap *Layer::getMask() const
{
    return this->mask;
}

bool Layer::isMaskActive() const
{
    return this->maskActive;
}

void Layer::setMaskActive(bool newMaskActive)
{
    this->maskActive = newMaskActive;
}


QBitmap *Layer::duplicateMask() const
{
    if(this->mask == NULL) return NULL;
    QBitmap *duplicate = new QBitmap(this->mask->size());
    *duplicate = *mask;
    return duplicate;
}

void Layer::serialize(QDataStream &stream)
{
    stream << this->getType();
    stream << this->name;
    stream << this->blendMode;
    stream << this->visibility;
    stream << this->opacity;
    stream << this->antialiasing;
    stream << (bool) (this->mask != NULL);
    if(this->mask != NULL) {
        stream << this->mask;
    }
    stream << this->maskActive;
}

void Layer::deserialize(QDataStream &stream)
{
    stream >> this->name;
    stream >> this->blendMode;
    stream >> this->visibility;
    stream >> this->opacity;
    stream >> this->antialiasing;
    bool mask_is_not_null;
    stream >> mask_is_not_null;
    if(mask_is_not_null) {
        QBitmap mask;
        stream >> mask;
        this->maskPaste(&mask);
    }
    stream >> this->maskActive;
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
