#include "bitmaplayer.h"

BitmapLayer::BitmapLayer(QObject *parent, const QString &name, const QSize &size) : Layer(parent, name)
{
    this->size = size;
    if(size.width() >= 1 && size.height() >= 1) {
        this->pixmap = QPixmap(size);
        this->painter = new QPainter(&this->pixmap);
        this->enableAntialiasing(this->isAntialiasingEnabled());
    }
    this->mouseHelper = MouseEventHelper(3);
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
        if(this->painter) {
            this->painter->end();
            delete this->painter;
        }
        this->painter = new QPainter(&this->pixmap);
        this->enableAntialiasing(this->isAntialiasingEnabled());
    }
}

const QPixmap &BitmapLayer::getPixmap() const
{
    return this->pixmap;
}

void BitmapLayer::paintEvent(QPainter &painter)
{
    painter.drawPixmap(
                0, 0,
                this->size.width(), this->size.height(),
                this->pixmap);
}

void BitmapLayer::mousePressEvent(const QPoint &pos)
{

}

void BitmapLayer::mouseReleaseEvent(const QPoint &pos)
{
    this->mouseHelper.resetMove();
}

void BitmapLayer::mouseDoubleClickEvent(const QPoint &pos)
{
}

void BitmapLayer::mouseMoveEvent(const QPoint &pos)
{
    if(this->mouseHelper.processMoveEvent(pos)) {
        // po definovanych vzdalenost dela tah
        QLine line = this->mouseHelper.lineFromLastPos();

        // TOOL
        painter->setPen(QPen(Qt::red, 3));
        painter->drawLine(line);
        // TOOL
    }
}

void BitmapLayer::outOfAreaEvent(const QPoint &pos)
{
    // dokonci tah
    const QPoint *last = this->mouseHelper.getLast();
    if(last) {
        QLine line(*last, pos);

        // TOOL
        painter->setPen(QPen(Qt::red, 3));
        painter->drawLine(line);
        // TOOL

        // reset
        this->mouseHelper.resetMove();
        // request repain
        this->requestRepaint();
    }
}
