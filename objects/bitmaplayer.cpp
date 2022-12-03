#include "bitmaplayer.h"

#include <QPainter>

BitmapLayer::BitmapLayer(QObject *parent, const QString &name, const QSize &size) : Layer(parent, name)
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

void BitmapLayer::paintEvent(QPainter &painter)
{
    /*
    painter.drawPixmap(
                offset.x(), offset.y(),
                this->size.width(), this->size.height(),
                this->pixmap);
                */

    // test
    painter.fillRect(
                20, 20,
                30, 50,
                QBrush(QColor(37, 37, 37), Qt::SolidPattern));
}
