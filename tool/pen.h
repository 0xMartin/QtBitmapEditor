#ifndef PEN_H
#define PEN_H

#include <QSpinBox>
#include <QSpacerItem>

#include "../utility/colorpicker.h"
#include "../base/tool.h"


class Pen : public Tool
{
public:
    Pen(QObject *parent, ColorPicker *colorPicker);

    ~Pen();

    virtual void mousePressEvent(const QPoint &pos);
    virtual void mouseReleaseEvent(const QPoint &pos);
    virtual void mouseDoubleClickEvent(const QPoint &pos);
    virtual void mouseMoveEvent(const QPoint &pos);
    virtual void outOfAreaEvent(const QPoint &pos);

protected:
    ColorPicker *colorPicker;
    QSpinBox *spinbox_size;
    QSpacerItem * spacerItem;
};

#endif // PEN_H
