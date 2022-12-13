#ifndef BRUSH_H
#define BRUSH_H

#include <QBrush>
#include <QSpinBox>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QComboBox>

#include "../utility/colorpicker.h"
#include "../utility/gradientwidget.h"
#include "../base/tool.h"
#include "../utility/mouseeventhelper.h"


#define TOOL_BRUSH 1002

/**
 * @brief Nastroj tuzka
 */
class Brush : public Tool
{
public:
    Brush(QObject *parent, ColorPicker *colorPicker);

    ~Brush();

    virtual void updateScale(float scale) override;

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
    QBrush brush;

    // UI controllers
    QVBoxLayout *layout;
    ColorPicker *colorPicker;
    QComboBox *comboBox_shape;
    GradientEditor *gradientEditor;
    QSpinBox *spinbox_size;
    QCheckBox *checkBox_Antialiasing;

    /**
     * @brief Po rovne care provede tah nastrojem stetec (Bresenham algoritmus)
     */
    void paintLineWithBrush(QPainter &painter, const QLineF &line);

    /**
     * @brief refreshBrush
     */
    void refreshBrush();
};

#endif // PEN_H
