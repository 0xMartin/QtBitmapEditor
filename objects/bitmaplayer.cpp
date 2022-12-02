#include "bitmaplayer.h"

#include <QPainter>

BitmapLayer::BitmapLayer(const QString &name, const QSize &size) : Layer(name)
{
    this->size = size;
    if(size.width() >= 1 && size.height() >= 1) {
        this->pixmap = QPixmap(size);
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
    }
}

const QPixmap &BitmapLayer::getPixmap() const
{
    return this->pixmap;
}

void BitmapLayer::paintEvent(QPainter &painter, const QPoint &offset)
{
    painter.drawPixmap(
                offset.x(), offset.y(),
                this->size.width(), this->size.height(),
                this->pixmap);
}
