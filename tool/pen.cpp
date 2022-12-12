#include "pen.h"

#include <QVBoxLayout>
#include <QPainterPath>

#include "../base/config.h"
#include "../layer/bitmaplayer.h"


Pen::Pen(QObject *parent, ColorPicker *colorPicker) : Tool(parent)
{
    this->name = tr("PEN");
    this->mouseHelper = MouseEventHelper(DEFAULT_MOUSE_HELPER_DIST);
    this->layout = new QVBoxLayout(this->ui);
    this->ui->setLayout(this->layout);
    this->colorPicker = colorPicker; 
    // velikost pera
    this->spinbox_size = new QSpinBox();
    this->spinbox_size->setPrefix(tr("Pen Size:"));
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
    this->pen = QPen(this->colorPicker->getColor(),
                     this->spinbox_size->value(),
                     Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
}

Pen::~Pen()
{
    if(this->layout) delete this->layout;
    if(this->spinbox_size) delete this->spinbox_size;
    if(this->checkBox_Antialiasing) delete this->checkBox_Antialiasing;
}

void Pen::updateScale(float scale)
{
    // update mouse helper
    this->mouseHelper.updateDistance(mapFunc(scale, 1.0, PIXEL_GRID_MIN_SCALE, DEFAULT_MOUSE_HELPER_DIST, 1.0));
}

void Pen::paintEvent(const QPointF &pos, float scale, QPainter &painter)
{
    // vykresleni tvaru a veliskota nastroje do horni nahledove vrstvy
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

/*****************************************************************************************/
// EVENTY PRO EDITACI BITMAPY


void Pen::mousePressEvent(const QPointF &pos)
{
    this->mouseHelper.processMoveEvent(pos);

    // refresh kresliciho nastroje
    int size = this->spinbox_size->value();
    this->pen = QPen(this->colorPicker->getColor(),
                     size,
                     Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);

    // vykreleni po dotiku
    BitmapLayer *layer = (BitmapLayer *)this->layerCheck(BITMAP_LAYER_TYPE);
    if(layer == NULL) return;
    this->painter.begin(&layer->pixmap);
    painter.setRenderHint(QPainter::Antialiasing, this->checkBox_Antialiasing->isChecked());
    this->painter.setPen(Qt::transparent);
    this->painter.setBrush(this->colorPicker->getColor());
    this->painter.drawEllipse(pos.x() - size/2, pos.y() - size/2, size, size);
    this->painter.end();
}

void Pen::mouseReleaseEvent(const QPointF &pos)
{
    this->mouseHelper.resetMove();
}

void Pen::mouseDoubleClickEvent(const QPointF &pos)
{

}

void Pen::mouseMoveEvent(const QPointF &pos)
{
    if(this->mouseHelper.processMoveEvent(pos)) {
        // po definovanych vzdalenost dela tah
        QLineF line = this->mouseHelper.lineFromLastPos();

        BitmapLayer *layer = (BitmapLayer *)this->layerCheck(BITMAP_LAYER_TYPE);
        if(layer == NULL) return;
        this->painter.begin(&layer->pixmap);
        painter.setRenderHint(QPainter::Antialiasing, this->checkBox_Antialiasing->isChecked());
        this->painter.setPen(this->pen);
        this->painter.drawLine(line);
        this->painter.end();
    }
}

void Pen::outOfAreaEvent(const QPointF &pos)
{
    // dokonci tah
    const QPointF *last = this->mouseHelper.getLast();
    if(last) {
        QLineF line(*last, pos);

        BitmapLayer *layer = (BitmapLayer *)this->layerCheck(BITMAP_LAYER_TYPE);
        if(layer == NULL) return;
        this->painter.begin(&layer->pixmap);
        painter.setRenderHint(QPainter::Antialiasing, this->checkBox_Antialiasing->isChecked());
        this->painter.setPen(this->pen);
        this->painter.drawLine(line);
        this->painter.end();

        // reset
        this->mouseHelper.resetMove();
        // request repain
        layer->requestRepaint();
    }
}
