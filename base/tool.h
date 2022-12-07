#ifndef TOOL_H
#define TOOL_H

#include <QObject>
#include <QWidget>
#include <QList>

#include "layer.h"

class Tool : public QObject
{
    Q_OBJECT
public:
    Tool(QObject *parent);

    virtual ~Tool();

    /**
     * @brief Priradi nastroji vrstvu se kterou bude manipulovat
     */
    void bindLayer(Layer *layer);

    /**
     * @brief Navrati UI tohoto nastroje
     * @return QLayout
     */
    QLayout *getUI();

    // events
    virtual void mousePressEvent(const QPoint &pos) = 0;
    virtual void mouseReleaseEvent(const QPoint &pos) = 0;
    virtual void mouseDoubleClickEvent(const QPoint &pos) = 0;
    virtual void mouseMoveEvent(const QPoint &pos) = 0;
    virtual void outOfAreaEvent(const QPoint &pos) = 0;

protected:
    // manipulovana vrstva
    Layer *layer;

    //ovladaci UI nastroje
    QLayout *ui;
};

#endif // TOOL_H
