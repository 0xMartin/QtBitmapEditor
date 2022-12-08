#ifndef ERASER_H
#define ERASER_H

#include <QSpinBox>
#include <QSpacerItem>

#include "../base/tool.h"
#include "../utility/mouseeventhelper.h"

/**
 * @brief Nastroj guma
 */
class Eraser : public Tool
{
public:
    Eraser(QObject *parent);
    ~Eraser();

    virtual void paintEvent(const QPoint &pos, float scale, QPainter &painter) override;
    virtual bool overLayerPainting() const override;

    virtual void mousePressEvent(const QPoint &pos) override;
    virtual void mouseReleaseEvent(const QPoint &pos) override;
    virtual void mouseDoubleClickEvent(const QPoint &pos) override;
    virtual void mouseMoveEvent(const QPoint &pos) override;
    virtual void outOfAreaEvent(const QPoint &pos) override;

protected:
    // helper pro mouse eventy
    MouseEventHelper mouseHelper;

    // Painter pro kresleni do vrstvy
    QPainter painter;

    // guma
    QPen pen;

    // UI controllers
    QSpinBox *spinbox_size;
    QSpacerItem * spacerItem;
};

#endif // ERASER_H