#ifndef ERASER_H
#define ERASER_H

#include <QPen>
#include <QSpinBox>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QCheckBox>

#include "../base/tool.h"
#include "../utility/mouseeventhelper.h"

#define TOOL_ERASER 1001

/**
 * @brief Nastroj guma
 */
class Eraser : public Tool
{
    Q_OBJECT
public:
    Eraser(QObject *parent);
    ~Eraser();

    virtual void updatTool(float scale) override;

    virtual void paintEvent(const QPointF &pos, float scale, QPainter &painter) override;
    virtual bool overLayerPainting() const override;
    virtual int getType() const override;

    virtual void mousePressEvent(const QPointF &pos) override;
    virtual void mouseReleaseEvent(const QPointF &pos) override;
    virtual void mouseDoubleClickEvent(const QPointF &pos) override;
    virtual void mouseMoveEvent(const QPointF &pos) override;
    virtual void outOfAreaEvent(const QPointF &pos) override;

protected:
    // helper pro mouse eventy
    MouseEventHelper mouseHelper;

    // Painter pro kresleni do vrstvy
    QPainter painter;

    // guma
    QPen pen;

    // UI controllers
    QVBoxLayout *layout;
    QSpinBox *spinbox_size;
    QCheckBox *checkBox_Antialiasing;
};

#endif // ERASER_H
