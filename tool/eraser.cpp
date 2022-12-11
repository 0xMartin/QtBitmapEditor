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
    this->spinbox_size->setPrefix(tr("Eraser Size:"));
    this->spinbox_size->setSuffix("px");
    this->spinbox_size->setMinimum(1);
    this->spinbox_size->setValue(10);
    this->spinbox_size->setMaximum(1000);
    this->layout->addWidget(this->spinbox_size);
    // antialiasing
    this->checkBox_Antialiasing = new QCheckBox();
    this->checkBox_Antialiasing->setChecked(true);
    this->checkBox_Antialiasing->setText(tr("Smooth Painting Mode"));
    this->layout->addWidget(this->checkBox_Antialiasing);
    // spacer
    this->layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding));

    // refresh
    this->pen = QPen(Qt::transparent,
                     this->spinbox_size->value(),
                     Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
}

Eraser::~Eraser()
{
    if(this->layout) delete this->layout;
    if(this->spinbox_size) delete this->spinbox_size;
    if(this->checkBox_Antialiasing) delete this->checkBox_Antialiasing;
}

void Eraser::paintEvent(const QPoint &pos, float scale, QPainter &painter)
{
    // vykresleni tvaru a veliskota nastroje do horni nahledove vrstvy
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
    int size = this->spinbox_size->value();

    // refresh kresliciho nastroje
    this->pen = QPen(Qt::transparent,
                     size,
                     Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);

    // vykreleni po dotiku
    BitmapLayer *layer = (BitmapLayer *)this->layerCheck(BITMAP_LAYER_TYPE);
    if(layer == NULL) return;
    this->painter.begin(&layer->pixmap);
    painter.setRenderHint(QPainter::Antialiasing, this->checkBox_Antialiasing->isChecked());
    this->painter.setBrush(Qt::transparent);
    this->painter.setCompositionMode(QPainter::CompositionMode_Clear);
    this->painter.drawEllipse(pos.x() - size/2, pos.y() - size/2, size, size);
    this->painter.end();
}

void Eraser::mouseReleaseEvent(const QPoint &pos)
{
    this->mouseHelper.resetMove();
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
        if(layer == NULL) return;
        this->painter.begin(&layer->pixmap);
        painter.setRenderHint(QPainter::Antialiasing, this->checkBox_Antialiasing->isChecked());
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
        if(layer == NULL) return;
        this->painter.begin(&layer->pixmap);
        painter.setRenderHint(QPainter::Antialiasing, this->checkBox_Antialiasing->isChecked());
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
