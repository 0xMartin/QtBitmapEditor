#include "gradientwidget.h"

#include <QColorDialog>

ColorPoint::ColorPoint(QColor color, float position)
{
    this->color = color;
    this->position = position;
}

GradientEditor::GradientEditor(QWidget *parent)
{

}

const QList<ColorPoint> &GradientEditor::getGradient() const
{
    return gradient;
}

void GradientEditor::clearGradient()
{

}

void GradientEditor::changeColor(qsizetype index)
{
    if(index >= this->gradient.length()) return;
    ColorPoint cpt = this->gradient.at(index);
    cpt.color = QColorDialog::getColor(cpt.color, parentWidget());
}

void GradientEditor::paintEvent(QPaintEvent *event)
{

}

void GradientEditor::mousePressEvent(QMouseEvent *event)
{

}

void GradientEditor::mouseReleaseEvent(QMouseEvent *event)
{

}

void GradientEditor::mouseMoveEvent(QMouseEvent *event)
{

}
