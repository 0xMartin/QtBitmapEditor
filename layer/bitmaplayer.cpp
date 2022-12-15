#include "bitmaplayer.h"

BitmapLayer::BitmapLayer(QObject *project, const QString &name, const QSize &size) : Layer(project, name)
{
    this->size = size;
    if(size.width() >= 1 && size.height() >= 1) {
        this->image = QImage(size, QImage::Format_ARGB32);
        this->image.fill(Qt::transparent);
    }
}

const QSize &BitmapLayer::getSize() const
{
    return this->size;
}

void BitmapLayer::setSize(const QSize &newSize)
{
    if(newSize.width() >= 1 && newSize.height() >= 1) {
        this->size = newSize;
        this->image = this->image.scaled(this->size.width(), this->size.height(), Qt::KeepAspectRatio);
        this->image.fill(Qt::transparent);
    }
}

void BitmapLayer::paintEvent(QPainter &painter)
{
    painter.drawImage(
                0, 0,
                this->image);
}

int BitmapLayer::getType()
{
    return BITMAP_LAYER_TYPE;
}

Layer *BitmapLayer::createDuplicate() const
{
    // vytvoreni duplikatu
    BitmapLayer *layer = new BitmapLayer(this->parent(), this->name, this->size);
    layer->mask = this->duplicateMask();
    layer->maskActive = this->maskActive;
    layer->opacity = this->opacity;
    layer->blendMode = this->blendMode;
    layer->antialiasing = this->antialiasing;
    // #############################
    layer->image = this->image;
    // #############################
    return layer;
}
