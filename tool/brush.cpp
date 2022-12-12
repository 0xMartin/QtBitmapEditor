#include "brush.h"

#include <QVBoxLayout>
#include <QPainterPath>

#include "../base/config.h"
#include "../layer/bitmaplayer.h"


Brush::Brush(QObject *parent, ColorPicker *colorPicker) : Tool(parent)
{
    this->name = tr("BRUSH");
    this->mouseHelper = MouseEventHelper(DEFAULT_MOUSE_HELPER_DIST);
    this->layout = new QVBoxLayout(this->ui);
    this->ui->setLayout(this->layout);
    this->colorPicker = colorPicker;
    // gradient
    this->gradientEditor = new GradientEditor();
    this->layout->addWidget(this->gradientEditor);
    // velikost stetce
    this->spinbox_size = new QSpinBox();
    this->spinbox_size->setPrefix(tr("Brush Size:"));
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
    this->brush = QBrush(this->colorPicker->getColor(), Qt::SolidPattern);
}

Brush::~Brush()
{
    if(this->layout) delete this->layout;
    if(this->spinbox_size) delete this->spinbox_size;
    if(this->checkBox_Antialiasing) delete this->checkBox_Antialiasing;
}

void Brush::updateScale(float scale)
{
    // update mouse helper
    this->mouseHelper.updateDistance(mapFunc(scale, 1.0, PIXEL_GRID_MIN_SCALE, DEFAULT_MOUSE_HELPER_DIST, 1.0));
}

void Brush::paintEvent(const QPointF &pos, float scale, QPainter &painter)
{
    // vykresleni tvaru a veliskota nastroje do horni nahledove vrstvy
    int size = this->spinbox_size->value();
    painter.setPen(Qt::black);
    float s = size * scale;
    painter.drawEllipse(pos.x() - s/2, pos.y() - s/2, s, s);
}

bool Brush::overLayerPainting() const
{
    return true;
}

int Brush::getType() const
{
    return TOOL_BRUSH;
}


/*****************************************************************************************/
// EVENTY PRO EDITACI BITMAPY

void Brush::mousePressEvent(const QPointF &pos)
{
    this->mouseHelper.processMoveEvent(pos);

    // refresh kresliciho nastroje
    int size = this->spinbox_size->value();
    this->brush = QBrush(this->colorPicker->getColor(), Qt::Dense1Pattern);

    // vykreleni po dotiku
    BitmapLayer *layer = (BitmapLayer *)this->layerCheck(BITMAP_LAYER_TYPE);
    if(layer == NULL) return;
    this->painter.begin(&layer->pixmap);
    painter.setRenderHint(QPainter::Antialiasing, this->checkBox_Antialiasing->isChecked());
    this->painter.setBrush(this->brush);
    this->painter.setPen(Qt::transparent);
    this->painter.drawEllipse(pos.x() - size/2, pos.y() - size/2, size, size);
    this->painter.end();
}

void Brush::mouseReleaseEvent(const QPointF &pos)
{
    this->mouseHelper.resetMove();
}

void Brush::mouseDoubleClickEvent(const QPointF &pos)
{

}

void Brush::mouseMoveEvent(const QPointF &pos)
{
    if(this->mouseHelper.processMoveEvent(pos)) {
        // po definovanych vzdalenost dela tah
        QLineF line = this->mouseHelper.lineFromLastPos();

        BitmapLayer *layer = (BitmapLayer *)this->layerCheck(BITMAP_LAYER_TYPE);
        if(layer == NULL) return;
        this->painter.begin(&layer->pixmap);
        painter.setRenderHint(QPainter::Antialiasing, this->checkBox_Antialiasing->isChecked());
        this->painter.setBrush(this->brush);
        this->paintLineWithBrush(this->painter, line);
        this->painter.end();
    }
}

void Brush::outOfAreaEvent(const QPointF &pos)
{
    // dokonci tah
    const QPointF *last = this->mouseHelper.getLast();
    if(last) {
        QLineF line(*last, pos);

        BitmapLayer *layer = (BitmapLayer *)this->layerCheck(BITMAP_LAYER_TYPE);
        if(layer == NULL) return;
        this->painter.begin(&layer->pixmap);
        painter.setRenderHint(QPainter::Antialiasing, this->checkBox_Antialiasing->isChecked());
        this->painter.setBrush(this->brush);
        this->paintLineWithBrush(this->painter, line);
        this->painter.end();

        // reset
        this->mouseHelper.resetMove();
        // request repain
        layer->requestRepaint();
    }
}

void Brush::paintLineWithBrush(QPainter &painter, const QLineF &line)
{
    this->painter.setBrush(this->brush);
    this->painter.setPen(Qt::transparent);
    int size = this->spinbox_size->value();


    int x0 = line.p1().x();
    int y0 = line.p1().y();
    int x1 = line.p2().x();
    int y1 = line.p2().y();

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        this->painter.drawEllipse(x0 - size/2, y0 - size/2, size, size);

        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}
