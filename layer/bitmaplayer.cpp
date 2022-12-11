#include "bitmaplayer.h"

BitmapLayer::BitmapLayer(QObject *parent, const QString &name, const QSize &size) : Layer(parent, name)
{
    this->size = size;
    if(size.width() >= 1 && size.height() >= 1) {
        this->pixmap = QPixmap(size);
        this->pixmap.fill(Qt::transparent);
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
        this->pixmap = this->pixmap.scaled(this->size.width(), this->size.height(), Qt::KeepAspectRatio);
        this->pixmap.fill(Qt::transparent);
    }
}

void BitmapLayer::paintEvent(QPainter &painter)
{
    painter.drawPixmap(
                0, 0,
                this->size.width(), this->size.height(),
                this->pixmap);
}

int BitmapLayer::getType()
{
    return BITMAP_LAYER_TYPE;
}

Layer *BitmapLayer::createDuplicate() const
{
    // vytvoreni duplikatu
    BitmapLayer *layer = new BitmapLayer(this->parent(), this->name, this->size);
    layer->opacity = this->opacity;
    layer->blendMode = this->blendMode;
    layer->antialiasing = this->antialiasing;
    layer->pixmap = this->pixmap;
    return layer;
}
