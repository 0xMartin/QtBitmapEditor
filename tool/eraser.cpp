#include "eraser.h"

#include <QVBoxLayout>
#include <QPainterPath>

#include "../layer/bitmaplayer.h"


Eraser::Eraser(QObject *parent) : Tool(parent)
{
    this->name = tr("ERASER");
    this->mouseHelper = MouseEventHelper(5);
    this->layout = new QVBoxLayout(this->ui);
    this->ui->setLayout(this->layout);
    // velikost pera
    this->spinbox_size = new QSpinBox();
    this->spinbox_size->setPrefix("Eraser Size:");
    this->spinbox_size->setSuffix("px");
    this->spinbox_size->setMinimum(1);
    this->spinbox_size->setValue(10);
    this->spinbox_size->setMaximum(1000);
    this->layout->addWidget(this->spinbox_size);

    // refresh
    this->pen = QPen(Qt::transparent,
                     this->spinbox_size->value(),
                     Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
}

Eraser::~Eraser()
{
    if(this->layout) delete this->layout;
    if(this->spinbox_size) delete this->spinbox_size;
}

void Eraser::paintEvent(const QPoint &pos, float scale, QPainter &painter)
{
    int size = this->spinbox_size->value();
    painter.setPen(Qt::black);
    float s = size * scale;
    painter.drawEllipse(pos.x() - s/2, pos.y() - s/2, s, s);
}

bool Eraser::overLayerPainting() const
{
    return true;
}

int Eraser::getType() const
{
    return TOOL_ERASER;
}

void Eraser::mousePressEvent(const QPoint &pos)
{
    // refresh kresliciho nastroje
    this->pen = QPen(Qt::transparent,
                     this->spinbox_size->value(),
                     Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
}

void Eraser::mouseReleaseEvent(const QPoint &pos)
{
    this->mouseHelper.resetMove();
    emit this->project->repaintSignal(this->project->getSelectedLayer());
}

void Eraser::mouseDoubleClickEvent(const QPoint &pos)
{

}

void Eraser::mouseMoveEvent(const QPoint &pos)
{
    if(this->mouseHelper.processMoveEvent(pos)) {
        // po definovanych vzdalenost dela tah
        QLine line = this->mouseHelper.lineFromLastPos();

        BitmapLayer *layer = (BitmapLayer *)this->layerCheck(BITMAP_LAYER_TYPE);
        this->painter.begin(&layer->pixmap);
        painter.setRenderHint(QPainter::Antialiasing, layer->isAntialiasingEnabled());
        this->painter.setPen(this->pen);
        this->painter.setCompositionMode(QPainter::CompositionMode_Clear);
        this->painter.drawLine(line);
        this->painter.end();
    }
}

void Eraser::outOfAreaEvent(const QPoint &pos)
{
    // dokonci tah
    const QPoint *last = this->mouseHelper.getLast();
    if(last) {
        QLine line(*last, pos);

        BitmapLayer *layer = (BitmapLayer *)this->layerCheck(BITMAP_LAYER_TYPE);
        this->painter.begin(&layer->pixmap);
        painter.setRenderHint(QPainter::Antialiasing, layer->isAntialiasingEnabled());
        this->painter.setPen(this->pen);
        this->painter.setCompositionMode(QPainter::CompositionMode_Clear);
        this->painter.drawLine(line);
        this->painter.end();

        // reset
        this->mouseHelper.resetMove();
        // request repain
        layer->requestRepaint();
    }
}
