#ifndef TEXT_H
#define TEXT_H

#include <QPen>
#include <QSpinBox>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QLabel>

#include "../base/tool.h"
#include "../utility/colorpicker.h"
#include "../utility/fontselector.h"
#include "../utility/mouseeventhelper.h"


#define TOOL_TEXT 1005

/**
 * @brief Nastroj pro pridavani textu
 */
class Text : public Tool
{
    Q_OBJECT
public:
    Text(QObject *parent);

    ~Text();

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

    // UI controllers
    QVBoxLayout *layout;
    ColorPicker *colorPicker;
    FontSelector *fontSelector;
    QSpinBox *spinbox_x;
    QSpinBox *spinbox_y;
    QCheckBox *checkBox_Antialiasing;

};

#endif // PEN_H
