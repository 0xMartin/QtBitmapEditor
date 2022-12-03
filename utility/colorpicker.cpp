#include "colorpicker.h"

#include <QColorDialog>

ColorPicker::ColorPicker( QWidget* parent ): QPushButton(parent)
{
    this->connect( this, SIGNAL(clicked()), this, SLOT(changeColor()));
    this->setToolTip(QString("Current color"));
    this->color.setRgb(0, 0, 0);
    this->updateColor();
}

void ColorPicker::updateColor()
{
    this->setStyleSheet("background-color: " + color.name());
}

void ColorPicker::changeColor()
{
    QColor newColor = QColorDialog::getColor(color, parentWidget());
    if (newColor != color)
    {
        setColor(newColor);
    }
}

void ColorPicker::setColor( const QColor& color )
{
    this->color = color;
    updateColor();
}

const QColor& ColorPicker::getColor() const
{
    return color;
}

void ColorPicker::changeEvent(QEvent *) {
    this->setFixedSize(QSize(25, 25));
}