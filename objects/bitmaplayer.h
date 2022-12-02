#ifndef BITMAPLAYER_H
#define BITMAPLAYER_H

#include "layer.h"

class BitmapLayer : public Layer
{
public:
    BitmapLayer(const QString &name);

    virtual void paintEvent(QPainter &painter, const QPoint &offset);
};

#endif // BITMAPLAYER_H
