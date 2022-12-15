#ifndef COLORPICKER_H
#define COLORPICKER_H


#include <QWidget>
#include <QBoxLayout>
#include <QColor>

/**
 * @brief Widget umoznuje zvolit barvu a tu nasled i pak zobrazi ve svem nahledu
 */
class ColorPicker : public QWidget
{
    Q_OBJECT
public:
    explicit ColorPicker(QWidget* parent);
    ~ColorPicker();

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

protected:
    virtual void mousePressEvent(QMouseEvent *event) override;

private:
    // aktualni barva
    QColor color;

    QBoxLayout* layout;
    QWidget *colorWidget;
};

#endif // COLORPICKER_H
