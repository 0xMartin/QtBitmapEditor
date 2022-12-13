#include "gradientwidget.h"

#include <QColorDialog>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QMenu>
#include <QAction>


#define SIDE_PADDING 8


ColorPoint::ColorPoint(QColor color, float position)
{
    this->color = color;
    this->position = position;
    this->selected = false;
}

GradientEditor::GradientEditor(QWidget *parent) : QWidget(parent)
{
    this->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed));
    this->setFixedHeight(70);
    this->gradient.append(new ColorPoint(Qt::red, 0.0));
    this->gradient.append(new ColorPoint(QColor(210, 20, 190), 1.0));
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)),
            this, SLOT(showContextMenu(QPoint)));
}

GradientEditor::~GradientEditor()
{
    this->clearGradient();
}

const QList<ColorPoint*> &GradientEditor::getGradient() const
{
    return gradient;
}

QLinearGradient GradientEditor::getAsLinearGradient(qreal xStart, qreal yStart, qreal xFinalStop, qreal yFinalStop) const
{
    QLinearGradient gradient(xStart, yStart, xFinalStop, yFinalStop);
    for(ColorPoint *pt : this->gradient) {
        if(pt == NULL) continue;
        gradient.setColorAt(pt->position, pt->color);
    }
    return gradient;
}

QRadialGradient GradientEditor::getAsRadialGradient(qreal cx, qreal cy, qreal radius) const
{
    QRadialGradient gradient(cx, cy, radius);
    for(ColorPoint *pt : this->gradient) {
        if(pt == NULL) continue;
        gradient.setColorAt(pt->position, pt->color);
    }
    return gradient;
}

QConicalGradient GradientEditor::getAsConicalGradient(qreal cx, qreal cy, qreal startAngle) const
{
    QConicalGradient gradient(cx, cy, startAngle);
    for(ColorPoint *pt : this->gradient) {
        if(pt == NULL) continue;
        gradient.setColorAt(pt->position, pt->color);
    }
    return gradient;
}

void GradientEditor::clearGradient()
{
    while (this->gradient.length() > 0) {
        ColorPoint *pt = this->gradient.takeLast();
        if(pt) delete pt;
    }
}

void GradientEditor::addColor()
{
    QColor c = QColorDialog::getColor(Qt::black, parentWidget());
    float p = (this->mouseHelper.getPress().x() - SIDE_PADDING) / (this->width() - 2 * SIDE_PADDING);
    p = qMax(0.0, qMin(p, 1.0));
    this->gradient.append(new ColorPoint(c, p));
    this->repaint();
}

void GradientEditor::changeColorOfSelected()
{
    for(ColorPoint *pt : this->gradient) {
        if(pt == NULL) continue;
        if(pt->selected) {
            pt->color = QColorDialog::getColor(pt->color, parentWidget());
            this->repaint();
            break;
        }
    }
}

void GradientEditor::removeSelected()
{
    qsizetype i = 0;
    for(ColorPoint *pt : this->gradient) {
        if(pt != NULL) {
            if(pt->selected) {
                ColorPoint *pt = this->gradient.takeAt(i);
                if(pt) delete pt;
                this->repaint();
                break;
            }
        }
        ++i;
    }
}

void GradientEditor::showContextMenu(const QPoint &pos)
{
    bool selected = false;
    for(ColorPoint *pt : this->gradient) {
        if(pt == NULL) continue;
        if(pt->selected) {
            selected = true;
            break;
        }
    }

    QMenu contextMenu(tr("Menu"), this);

    if(selected) {
        // menu pro vybranou barvu
        QAction action1(tr("Change Color"), this);
        connect(&action1, SIGNAL(triggered()), this, SLOT(changeColorOfSelected()));
        contextMenu.addAction(&action1);
        QAction action2(tr("Remove"), this);
        connect(&action2, SIGNAL(triggered()), this, SLOT(removeSelected()));
        contextMenu.addAction(&action2);
        contextMenu.exec(this->mapToGlobal(pos));
    } else {
        // menu pro widget
        QAction action1(tr("Add Color"), this);
        connect(&action1, SIGNAL(triggered()), this, SLOT(addColor()));
        contextMenu.addAction(&action1);
        QAction action2(tr("Clear"), this);
        connect(&action2, SIGNAL(triggered()), this, SLOT(clearGradient()));
        contextMenu.addAction(&action2);
        contextMenu.exec(this->mapToGlobal(pos));
    }
}

void GradientEditor::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    const int width = this->width() - 2 * SIDE_PADDING;
    const int height = 40;

    // bg
    painter.fillRect(SIDE_PADDING, 0, width, height, QBrush(Qt::white, Qt::SolidPattern));
    QBrush b(QColor(200, 200, 200), Qt::SolidPattern);
    int x_end = width + SIDE_PADDING;
    int y_end = height;
    const float step = 7;
    float step2 = 2 * step;
    float x_offset, step_x, step_y;
    for(int y= 0, i = 0; (int)y < y_end; y += step) {
        x_offset = i++ % 2 == 0 ? 0.0f : step;
        step_y = y + (int)step < y_end ? step : (y_end - y);
        for(int x = x_offset + SIDE_PADDING; (int)x < x_end; x += step2) {
            step_x = x + (int)step < x_end ? step : (x_end - x);
            painter.fillRect(x, y, step_x, step_y, b);
        }
    }

    // colors
    QLinearGradient gradient(0, 0, width, 0);
    for(ColorPoint *pt : this->gradient) {
        if(pt == NULL) continue;
        gradient.setColorAt(pt->position, pt->color);
    }
    painter.fillRect(SIDE_PADDING, 0, width, height, QBrush(gradient));

    // outline
    painter.setPen(Qt::black);
    painter.drawRect(SIDE_PADDING, 0, width, height);

    // color marks
    for(ColorPoint *pt : this->gradient) {
        if(pt == NULL) continue;
        int x = SIDE_PADDING + pt->position * width;
        if(pt->selected) {
            painter.setPen(QColor(255, 194, 52));
        } else {
            painter.setPen(Qt::black);
        }
        painter.fillRect(x - 7, 42, 14, 17, QBrush(pt->color, Qt::SolidPattern));
        painter.drawRect(x - 7, 42, 14, 17);
        QPainterPath path;
        path.moveTo(QPoint(x - 7, 42));
        path.lineTo(QPoint(x, 33));
        path.lineTo(QPoint(x + 7, 42));
        if(pt->selected) {
            painter.fillPath(path, QBrush(QColor(183, 134, 32), Qt::SolidPattern));
        } else {
            painter.fillPath(path, QBrush(Qt::black, Qt::SolidPattern));

        }
    }

}

void GradientEditor::mousePressEvent(QMouseEvent *event)
{
    const int width = this->width() - 2 * SIDE_PADDING;
    for(ColorPoint *pt : this->gradient) {
        if(pt == NULL) continue;
        int x = SIDE_PADDING + pt->position * width;
        QRect r(x - 7, 39, 14, 20);
        pt->selected = r.contains(event->pos());
    }
    this->repaint();

    this->mouseHelper.processPressEvent(event->pos());
}

void GradientEditor::mouseReleaseEvent(QMouseEvent *event)
{
    this->mouseHelper.resetMove();
}

void GradientEditor::mouseMoveEvent(QMouseEvent *event)
{
    if(this->mouseHelper.processMoveEvent(event->pos())) {
        QPointF diff = this->mouseHelper.diffFromLastPos();
        for(ColorPoint *pt : this->gradient) {
            if(pt == NULL) continue;
            if(pt->selected) {
                pt->position += diff.x() / (this->width() - 2 * SIDE_PADDING);
                if(pt->position < 0.0) {
                    pt->position = 0.0;
                }
                if(pt->position > 1.0) {
                    pt->position = 1.0;
                }
                this->repaint();
                break;
            }
        }
    }
}
