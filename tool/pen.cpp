#include "pen.h"

#include <QVBoxLayout>


Pen::Pen(QObject *parent, ColorPicker *colorPicker) : Tool(parent)
{
    this->colorPicker = colorPicker;
    this->ui = new QVBoxLayout();
    this->spinbox_size = new QSpinBox();
    // velikost pera
    this->spinbox_size->setPrefix("Pen Size:");
    this->spinbox_size->setSuffix("px");
    this->spinbox_size->setMinimum(1);
    this->spinbox_size->setValue(10);
    this->spinbox_size->setMaximum(500);
    this->ui->addWidget(this->spinbox_size);
    //spacer
    this->spacerItem = new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding);
    ((QVBoxLayout*)this->ui)->addSpacerItem(this->spacerItem);
}

Pen::~Pen()
{
    if(this->ui) delete this->ui;
    if(this->spinbox_size) delete this->spinbox_size;
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
