#include "workspace.h"

#include <QPainter>

Workspace::Workspace(QWidget *parent): QWidget(parent)
{
    this->setBackgroundRole(QPalette::Base);
    this->setAutoFillBackground(true);
    this->setMinimumSize(QSize(400, 400));
}

void Workspace::setProject(Project *project) {
    this->project = project;
}

Project *Workspace::getProject() const {
    return this->project;
}

void Workspace::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    QRectF rectangle(10.0, 20.0, 80.0, 60.0);
    int startAngle = 30 * 16;
    int spanAngle = 120 * 16;
    painter.drawArc(rectangle, startAngle, spanAngle);
}
