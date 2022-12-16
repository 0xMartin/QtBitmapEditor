#ifndef PENCIL_H
#define PENCIL_H

#include <QPen>
#include <QSpinBox>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QLabel>

#include "../utility/colorpicker.h"
#include "../base/tool.h"
#include "../utility/mouseeventhelper.h"


#define TOOL_PENCIL 1000

/**
 * @brief Nastroj tuzka
 */
class Pencil : public Tool
{
    Q_OBJECT
public:
    Pencil(QObject *parent, ColorPicker *colorPicker);

    ~Pencil();

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

    // tuzka
    QPen pen;

    // UI controllers
    QVBoxLayout *layout;
    ColorPicker *colorPicker;
    QSpinBox *spinbox_size;
    QCheckBox *checkBox_Antialiasing;

};

#endif // PEN_H
