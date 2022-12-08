#ifndef PEN_H
#define PEN_H

#include <QSpinBox>
#include <QSpacerItem>

#include "../utility/colorpicker.h"
#include "../base/tool.h"
#include "../utility/mouseeventhelper.h"


#define TOOL_PEN 1000

/**
 * @brief Nastroj tuzka
 */
class Pen : public Tool
{
public:
    Pen(QObject *parent, ColorPicker *colorPicker);

    ~Pen();

    virtual void paintEvent(const QPoint &pos, float scale, QPainter &painter) override;
    virtual bool overLayerPainting() const override;
    virtual int getType() const override;

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

    // tuzka
    QPen pen;

    // UI controllers
    ColorPicker *colorPicker;
    QSpinBox *spinbox_size;
    QSpacerItem * spacerItem;
};

#endif // PEN_H
