#include "bitmaplayer.h"

BitmapLayer::BitmapLayer(QObject *parent, const QString &name, const QSize &size) : Layer(parent, name)
{
    this->size = size;
    if(size.width() >= 1 && size.height() >= 1) {
        this->pixmap = QPixmap(size);
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
        this->painter.end();
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
        this->painter.begin(&this->pixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        this->painter.setPen(QPen(Qt::red, 3));
        this->painter.drawLine(line);
        this->painter.end();
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
        this->painter.begin(&this->pixmap);
        painter.setRenderHint(QPainter::Antialiasing);
        this->painter.setPen(QPen(Qt::red, 3));
        this->painter.drawLine(line);
        this->painter.end();
        // TOOL

        // reset
        this->mouseHelper.resetMove();
        // request repain
        this->requestRepaint();
    }
}
