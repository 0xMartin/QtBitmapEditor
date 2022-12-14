#ifndef EYEDROPPER_H
#define EYEDROPPER_H

#include <QPen>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QListWidget>
#include <ctime>

#include "../utility/colorpicker.h"
#include "../base/tool.h"


#define TOOL_EYEDROPPER 1004


/**
 * @brief Nastroj pro ziskani barvy z workspacu
 */
class EyeDropper : public Tool
{
    Q_OBJECT
public:
    EyeDropper(QObject *parent, ColorPicker *colorPicker);
    ~EyeDropper();

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
    // buffer image z pred renderovanym projekt
    QImage buffer;

    // posledni cast updatu buffer image
    time_t updateTime;

    // pozice v projektu
    QPoint pos;

    // UI controllers
    QVBoxLayout *layout;
    ColorPicker *colorPicker;

    QListWidget *listWidget;

};

#endif // PEN_H
