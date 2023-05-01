#ifndef COLORITEM_H
#define COLORITEM_H


#include <QWidget>
#include <QColor>
#include <QHBoxLayout>
#include <QLabel>

#include "colorpicker.h"

/**
 * @brief List item s barvou, po kliknuti na nej se barva nastavy i na definovany color picker
 */
class ColorItem : public QWidget {
public:
    /**
     * @brief Vytvori color item
     * @param color - Barva color itemu
     * @param colorPicker - Pointer na externi color picker
     * @param size - Velikost Color itemu (velikost barevne plochy)
     * @param parent - QWidget parent
     */
    ColorItem(const QColor &color, ColorPicker *colorPicker, const QSize &size, QWidget *parent);
    ~ColorItem();

protected:
    QColor color;
    ColorPicker *colorPicker;

    QHBoxLayout *layout;
    QLabel *colorLabel;
    QLabel *label;

    virtual void mousePressEvent(QMouseEvent *event) override;
};

#endif // COLORITEM_H
