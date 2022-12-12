#include "gradientwidget.h"

#include <QColorDialog>
#include <QPainter>
#include <QPainterPath>


ColorPoint::ColorPoint(QColor color, float position)
{
    this->color = color;
    this->position = position;
}

GradientEditor::GradientEditor(QWidget *parent) : QWidget(parent)
{
    this->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
    this->setFixedHeight(70);
    this->gradient.append(ColorPoint(Qt::red, 0.0));
    this->gradient.append(ColorPoint(QColor(210, 20, 190, 120), 1.0));
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

void GradientEditor::changePosition(qsizetype index, float position)
{
    ColorPoint cpt = this->gradient.at(index);
    cpt.position = position;
}

void GradientEditor::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const int side_padding = 8;
    const int width = this->width() - 2 * side_padding;
    const int height = 40;

    // bg
    painter.fillRect(side_padding, 0, width, height, QBrush(Qt::white, Qt::SolidPattern));
    QBrush b(QColor(200, 200, 200), Qt::SolidPattern);
    int x_end = width + side_padding;
    int y_end = height;
    const float step = 7;
    float step2 = 2 * step;
    float x_offset, step_x, step_y;
    for(int y= 0, i = 0; (int)y < y_end; y += step) {
        x_offset = i++ % 2 == 0 ? 0.0f : step;
        step_y = y + (int)step < y_end ? step : (y_end - y);
        for(int x = x_offset + side_padding; (int)x < x_end; x += step2) {
            step_x = x + (int)step < x_end ? step : (x_end - x);
            painter.fillRect(x, y, step_x, step_y, b);
        }
    }

    // colors
    QLinearGradient gradient(0, 0, width, 0);
    for(ColorPoint pt : this->gradient) {
        gradient.setColorAt(pt.position, pt.color);
    }
    painter.fillRect(side_padding, 0, width, height, QBrush(gradient));

    // color marks
    painter.setPen(Qt::black);
    for(ColorPoint pt : this->gradient) {
        int x = side_padding + pt.position * width;
        painter.fillRect(x - 7, 42, 14, 17, QBrush(pt.color, Qt::SolidPattern));
        painter.drawRect(x - 7, 42, 14, 17);
        QPainterPath path;
        path.moveTo(QPoint(x - 7, 42));
        path.lineTo(QPoint(x, 33));
        path.lineTo(QPoint(x + 7, 42));
        painter.fillPath(path, QBrush(Qt::black, Qt::SolidPattern));
    }

    // outline
    painter.drawRect(side_padding, 0, width, height);
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
