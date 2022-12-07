#include "pen.h"

#include <QVBoxLayout>


Pen::Pen(QObject *parent) : Tool(parent)
{
    this->ui = new QVBoxLayout();
}

Pen::~Pen()
{
    if(this->ui) delete this->ui;
}

void Pen::mousePressEvent(const QPoint &pos)
{

}

void Pen::mouseReleaseEvent(const QPoint &pos)
{

}

void Pen::mouseDoubleClickEvent(const QPoint &pos)
{

}

void Pen::mouseMoveEvent(const QPoint &pos)
{

}

void Pen::outOfAreaEvent(const QPoint &pos)
{

}
