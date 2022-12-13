#ifndef COLORPICKER_H
#define COLORPICKER_H

#include <QPushButton>
#include <QColor>

/**
 * @brief Widget umoznuje zvolit barvu a tu nasled i pak zobrazi ve svem nahledu
 */
class ColorPicker : public QPushButton
{
    Q_OBJECT
public:
    explicit ColorPicker(QWidget* parent);

    /**
     * @brief Nastavi novou barvu
     * @param color - Nova barva
     */
    void setColor(const QColor& color);

    /**
     * @brief Navrati aktualni barvu
     * @return QColor
     */
    const QColor& getColor() const;

    virtual void changeEvent(QEvent * event);

public slots:
    /**
     * @brief Obnoveni bravy
     */
    void updateColor();

    /**
     * @brief Otevre dialog pro zvoleni nove bravy
     */
    void changeColor();

signals:
    /**
     * @brief Signal je vyvolan ve chvili kdy dojde ke zmene barvy
     * @param color - Nova barva
     */
    void colorChange(const QColor& color);

private:
    // aktualni barva
    QColor color;
};

#endif // COLORPICKER_H
