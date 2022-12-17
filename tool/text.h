#ifndef TEXT_H
#define TEXT_H

#include <QSpinBox>
#include <QLineEdit>
#include <QSpacerItem>
#include <QVBoxLayout>
#include <QCheckBox>
#include <QLabel>

#include "../base/tool.h"
#include "../utility/colorpicker.h"
#include "../utility/fontselector.h"

#define TOOL_TEXT 1005

/**
 * @brief Nastroj pro pridavani a editaci textu
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
    // UI controllers
    QVBoxLayout *layout;
    QLineEdit *lineEdit_text;
    ColorPicker *colorPicker;
    FontSelector *fontSelector;
    QSpinBox *spinbox_x;
    QSpinBox *spinbox_y;
    QCheckBox *checkBox_Antialiasing;

private slots:
    void on_text_textChanged(const QString &text);
    void on_colorPicker_colorChange(const QColor &color);
    void on_spinbox_x_valueChanged(int val);
    void on_spinbox_y_valueChanged(int val);
    void on_fontSelector_fontChanged(const QFont &font);

};

#endif // PEN_H
