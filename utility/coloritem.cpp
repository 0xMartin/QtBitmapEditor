#include "coloritem.h"

#include <QPainter>
#include <QSpacerItem>

ColorItem::ColorItem(const QColor &color, ColorPicker *colorPicker, const QSize &size, QWidget *parent) : QWidget(parent)
{
    this->color = color;
    this->colorPicker = colorPicker;

    // UI
    this->setFixedHeight(size.height());
    this->layout = new QHBoxLayout(this);

    // color
    this->colorLabel = new QLabel(this);
    QPixmap pixmap(size);
    QPainter painter(&pixmap);
    painter.fillRect(0, 0, size.width(), size.height(), QBrush(Qt::white, Qt::SolidPattern));
    painter.fillRect(0, 0, size.width(), size.height(), QBrush(color, Qt::SolidPattern));
    this->colorLabel->setPixmap(pixmap);
    this->layout->addWidget(this->colorLabel);

    // color info
    this->label = new QLabel(this);
    this->label->setAlignment(Qt::AlignLeft);
    QString colorInfo = QString("RGB(%1, %2, %3)").arg(
                QString::number(color.red()),
                QString::number(color.green()),
                QString::number(color.blue()));
    colorInfo += " " + color.name().toUpper();
    this->label->setText(colorInfo);
    this->layout->addWidget(this->label);

    // spacer
    this->layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed));

    this->setLayout(layout);
}

ColorItem::~ColorItem()
{
    if(this->layout) delete this->layout;
    if(this->colorLabel) delete this->colorLabel;
    if(this->label) delete this->label;
}

void ColorItem::mousePressEvent(QMouseEvent *event)
{
    this->colorPicker->setColor(this->color);
}
