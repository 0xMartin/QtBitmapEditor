#ifndef FILLCOLOR_H
#define FILLCOLOR_H

#include <QSpinBox>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QLabel>
#include <QSlider>

#include "../utility/colorpicker.h"
#include "../base/tool.h"


#define TOOL_FILLCOLOR 1003

/**
 * @brief Nastroj pro vyplneni barvou
 */
class FillColor : public Tool
{
    Q_OBJECT
public:
    FillColor(QObject *parent, ColorPicker *colorPicker);

    ~FillColor();

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
    // Painter pro kresleni do vrstvy
    QPainter painter;

    // UI controllers
    QVBoxLayout *layout;
    ColorPicker *colorPicker;
    QSpinBox *spinbox_tolerance_red;
    QSpinBox *spinbox_tolerance_green;
    QSpinBox *spinbox_tolerance_blue;
    QSpinBox *spinbox_tolerance_alpha;
    QSlider *slider_tolerance_all;

private:
    void floodFill(QImage &image, const QPoint &start, const QColor &color,
                   int toleranceR, int toleranceG, int toleranceB, int tolerance);

private slots:
    void valueChanged(int value);

};

#endif // PEN_H
