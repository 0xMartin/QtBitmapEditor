#include "colorpicker.h"

#include <QColorDialog>

ColorPicker::ColorPicker(QWidget* parent): QWidget(parent)
{
    this->setToolTip(tr("Select Color"));
    this->setCursor(Qt::PointingHandCursor);
    this->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    this->layout = new QBoxLayout(QBoxLayout::LeftToRight, this);

    this->colorWidget = new QWidget(this);
    this->colorWidget->setSizePolicy(QSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed));
    this->colorWidget->setFixedSize(QSize(25, 25));
    this->layout->addWidget(this->colorWidget, Qt::AlignCenter);

    this->color.setRgb(0, 0, 0);
    this->updateColor();
}

ColorPicker::~ColorPicker() {
    if(this->layout) delete this->layout;
    if(this->colorWidget) delete this->colorWidget;
}

void ColorPicker::updateColor()
{
    this->colorWidget->setStyleSheet("background-color: " + color.name());
}

void ColorPicker::changeColor()
{
    QColor newColor = QColorDialog::getColor(color, parentWidget());
    if (newColor != color)
    {
        this->setColor(newColor);
    }
}

void ColorPicker::setColor(const QColor& color)
{
    this->color = color;
    emit this->colorChange(color);
    this->updateColor();
}

const QColor& ColorPicker::getColor() const
{
    return this->color;
}

void ColorPicker::mousePressEvent(QMouseEvent *event)
{
    this->changeColor();
}
