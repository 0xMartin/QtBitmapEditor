#ifndef PEN_H
#define PEN_H

#include "../base/tool.h"


class Pen : public Tool
{
public:
    Pen(QObject *parent);

    ~Pen();

    virtual void mousePressEvent(const QPoint &pos);
    virtual void mouseReleaseEvent(const QPoint &pos);
    virtual void mouseDoubleClickEvent(const QPoint &pos);
    virtual void mouseMoveEvent(const QPoint &pos);
    virtual void outOfAreaEvent(const QPoint &pos);

protected:
};

#endif // PEN_H
