#include "pen.h"

#include <QVBoxLayout>
#include <QPainterPath>

#include "../layer/bitmaplayer.h"


Pen::Pen(QObject *parent, ColorPicker *colorPicker) : Tool(parent)
{
    this->colorPicker = colorPicker;
    this->ui = new QVBoxLayout();
    this->spinbox_size = new QSpinBox();
    this->mouseHelper = MouseEventHelper(5);
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

    // refresh
    this->pen = QPen(this->colorPicker->getColor(),
                     this->spinbox_size->value(),
                     Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
}

Pen::~Pen()
{
    if(this->ui) delete this->ui;
    if(this->spinbox_size) delete this->spinbox_size;
}

void Pen::paintEvent(const QPoint &pos, float scale, QPainter &painter)
{
    int size = this->spinbox_size->value();
    painter.setPen(Qt::black);
    float s = size * scale;
    painter.drawEllipse(pos.x() - s/2, pos.y() - s/2, s, s);
}

bool Pen::overLayerPainting() const
{
    return true;
}

int Pen::getType() const
{
    return TOOL_PEN;
}

void Pen::mousePressEvent(const QPoint &pos)
{
    // refresh kresliciho nastroje
    this->pen = QPen(this->colorPicker->getColor(),
                     this->spinbox_size->value(),
                     Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
}

void Pen::mouseReleaseEvent(const QPoint &pos)
{
    this->mouseHelper.resetMove();
    emit this->project->repaintSignal(this->project->getSelectedLayer());
}

void Pen::mouseDoubleClickEvent(const QPoint &pos)
{

}

void Pen::mouseMoveEvent(const QPoint &pos)
{
    if(this->mouseHelper.processMoveEvent(pos)) {
        // po definovanych vzdalenost dela tah
        QLine line = this->mouseHelper.lineFromLastPos();

        BitmapLayer *layer = (BitmapLayer *)this->layerCheck(BITMAP_LAYER_TYPE);
        this->painter.begin(&layer->pixmap);
        painter.setRenderHint(QPainter::Antialiasing, layer->isAntialiasingEnabled());
        this->painter.setPen(this->pen);
        this->painter.drawLine(line);
        this->painter.end();
    }
}

void Pen::outOfAreaEvent(const QPoint &pos)
{
    // dokonci tah
    const QPoint *last = this->mouseHelper.getLast();
    if(last) {
        QLine line(*last, pos);

        BitmapLayer *layer = (BitmapLayer *)this->layerCheck(BITMAP_LAYER_TYPE);
        this->painter.begin(&layer->pixmap);
        painter.setRenderHint(QPainter::Antialiasing, layer->isAntialiasingEnabled());
        this->painter.setPen(this->pen);
        this->painter.drawLine(line);
        this->painter.end();

        // reset
        this->mouseHelper.resetMove();
        // request repain
        layer->requestRepaint();
    }
}
