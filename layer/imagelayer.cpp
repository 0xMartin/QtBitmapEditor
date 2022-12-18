#include "imagelayer.h"

#include "../base/project.h"

ImageLayer::ImageLayer(QObject *project) : Layer(project, "")
{
}

ImageLayer::ImageLayer(QObject *project, const QString &name, const QString &URL, const QSize &size) : Layer(project, name)
{
    this->size = size;
    this->position = QPoint(0, 0);
    this->URL = URL;
    if(size.width() >= 1 && size.height() >= 1) {
        this->image = QImage(((Project*)project)->getDirPath() + URL);
        this->image = this->image.scaled(
                    (this->size.width() / 100) * this->image.width(),
                    (this->size.height() / 100) * this->image.height(),
                    Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }
}

const QSize &ImageLayer::getSize() const
{
    return this->size;
}

void ImageLayer::setSize(const QSize &newSize)
{
    if(newSize.width() >= 1 && newSize.height() >= 1) {
        this->size = newSize;
        this->image = QImage(((Project*)this->parent())->getDirPath() + URL);
        this->image = this->image.scaled(
                    (this->size.width() / 100) * this->image.width(),
                    (this->size.height() / 100) * this->image.height(),
                    Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }
}

void ImageLayer::paintEvent(QPainter &painter)
{
    painter.drawImage(this->position, this->image);
}

qint32 ImageLayer::getType() const
{
    return IMAGE_LAYER_TYPE;
}

Layer *ImageLayer::createDuplicate() const
{
    // vytvoreni duplikatu
    ImageLayer *layer = new ImageLayer(this->parent(), this->name, this->URL, this->size);
    layer->mask = this->duplicateMask();
    layer->maskActive = this->maskActive;
    layer->opacity = this->opacity;
    layer->blendMode = this->blendMode;
    layer->antialiasing = this->antialiasing;
    // #############################
    layer->position = this->position;
    // #############################
    return layer;
}

void ImageLayer::serialize(QDataStream &stream)
{
    Layer::serialize(stream);
    stream << this->size;
    stream << this->URL;
}

void ImageLayer::deserialize(QDataStream &stream)
{
    Layer::deserialize(stream);
    stream >> this->size;
    stream >> this->URL;
}
