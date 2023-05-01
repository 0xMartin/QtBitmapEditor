#include "fillcolor.h"

#include <QVBoxLayout>
#include <QPainterPath>
#include <QMessageBox>
#include <queue>

#include "../layer/bitmaplayer.h"
#include "../base/config.h"


FillColorTool::FillColorTool(QObject *parent, ColorPicker *colorPicker) : Tool(parent)
{
    this->name = tr("FILL COLOR");
    this->colorPicker = colorPicker;

    //****************************************************************************************
    // sestaveni UI pro ovladani nastroje
    //****************************************************************************************
    this->ui = new QWidget();
    this->layout = new QVBoxLayout(this->ui);
    this->ui->setLayout(this->layout);

    // tolerance slider
    this->slider_tolerance_all = new QSlider(Qt::Horizontal);
    this->slider_tolerance_all->setTickInterval(20);
    this->slider_tolerance_all->setMinimum(0);
    this->slider_tolerance_all->setMaximum(100);
    this->slider_tolerance_all->setValue(DEFAULT_FILL_TOLERANCE);
    this->layout->addWidget(this->slider_tolerance_all);
    connect(this->slider_tolerance_all, SIGNAL(valueChanged(int)), this, SLOT(valueChanged(int)));

    // tolerance (red)
    this->spinbox_tolerance_red = new QSpinBox();
    this->spinbox_tolerance_red->setPrefix(tr("Tolerance Red:"));
    this->spinbox_tolerance_red->setSuffix(tr("%"));
    this->spinbox_tolerance_red->setMinimum(0);
    this->spinbox_tolerance_red->setMaximum(100);
    this->spinbox_tolerance_red->setValue(DEFAULT_FILL_TOLERANCE);
    this->layout->addWidget(this->spinbox_tolerance_red);

    // tolerance (green)
    this->spinbox_tolerance_green = new QSpinBox();
    this->spinbox_tolerance_green->setPrefix(tr("Tolerance Green:"));
    this->spinbox_tolerance_green->setSuffix(tr("%"));
    this->spinbox_tolerance_green->setMinimum(0);
    this->spinbox_tolerance_green->setMaximum(100);
    this->spinbox_tolerance_green->setValue(DEFAULT_FILL_TOLERANCE);
    this->layout->addWidget(this->spinbox_tolerance_green);

    // tolerance (blue)
    this->spinbox_tolerance_blue = new QSpinBox();
    this->spinbox_tolerance_blue->setPrefix(tr("Tolerance Blue:"));
    this->spinbox_tolerance_blue->setSuffix(tr("%"));
    this->spinbox_tolerance_blue->setMinimum(0);
    this->spinbox_tolerance_blue->setMaximum(100);
    this->spinbox_tolerance_blue->setValue(DEFAULT_FILL_TOLERANCE);
    this->layout->addWidget(this->spinbox_tolerance_blue);

    // tolerance (alpha)
    this->spinbox_tolerance_alpha = new QSpinBox();
    this->spinbox_tolerance_alpha->setPrefix(tr("Tolerance Alpha:"));
    this->spinbox_tolerance_alpha->setSuffix(tr("%"));
    this->spinbox_tolerance_alpha->setMinimum(0);
    this->spinbox_tolerance_alpha->setMaximum(100);
    this->spinbox_tolerance_alpha->setValue(DEFAULT_FILL_TOLERANCE);
    this->layout->addWidget(this->spinbox_tolerance_alpha);

    // spacer
    this->layout->addSpacerItem(
                new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding));
}

FillColorTool::~FillColorTool()
{
    if(this->layout) delete this->layout;
    if(this->spinbox_tolerance_red) delete this->spinbox_tolerance_red;
    if(this->spinbox_tolerance_green) delete this->spinbox_tolerance_green;
    if(this->spinbox_tolerance_blue) delete this->spinbox_tolerance_blue;
    if(this->spinbox_tolerance_alpha) delete this->spinbox_tolerance_alpha;
    if(this->slider_tolerance_all) delete this->slider_tolerance_all;
}

void FillColorTool::updatTool(float scale)
{
}

void FillColorTool::paintEvent(const QPointF &pos, float scale, QPainter &painter)
{
}

bool FillColorTool::overLayerPainting() const
{
    return false;
}

int FillColorTool::getType() const
{
    return TOOL_FILLCOLOR;
}


/*****************************************************************************************/
// EVENTY PRO EDITACI BITMAPY
/*****************************************************************************************/

void FillColorTool::mousePressEvent(const QPointF &pos)
{
    BitmapLayer *layer = (BitmapLayer *)this->layerCheck(BITMAP_LAYER_TYPE);

    if(layer != NULL) {
        int tR = (this->spinbox_tolerance_red->value() / 100.0) * 255;
        int tG = (this->spinbox_tolerance_green->value() / 100.0) * 255;
        int tB = (this->spinbox_tolerance_blue->value() / 100.0) * 255;
        int tA = (this->spinbox_tolerance_alpha->value() / 100.0) * 255;

        this->floodFill(
                    layer->image,
                    QPoint(pos.x(), pos.y()),
                    this->colorPicker->getColor(),
                    tR, tG, tB, tA);
    } else {
        QMessageBox::warning(
                    this->ui,
                    tr("Fill Color Tool"),
                    tr("The selected layer is not a bitmap format! It must be converted to bitmap format."));
        return;
    }
}

void FillColorTool::mouseReleaseEvent(const QPointF &pos)
{
}

void FillColorTool::mouseDoubleClickEvent(const QPointF &pos)
{
}

void FillColorTool::mouseMoveEvent(const QPointF &pos)
{
}

void FillColorTool::outOfAreaEvent(const QPointF &pos)
{
}


/*****************************************************************************************/
// PRIVATNI FUNKCE
/*****************************************************************************************/

void FillColorTool::floodFill(QImage &image, const QPoint &start, const QColor &color,
                          int toleranceR, int toleranceG, int toleranceB, int toleranceA)
{
    const QRgb startColor = image.pixel(start);
    if(startColor == color.rgba()) return;

    std::queue<QPoint> queue;
    queue.push(start);

    QRgb fillColor = color.rgba();
    QRgb current;
    QPoint p;
    while (!queue.empty()) {
        p = queue.front();
        queue.pop();

        if (image.rect().contains(p)) {
            current = image.pixel(p);
            if(current == fillColor) continue;
            if(abs(qRed(current) - qRed(startColor)) > toleranceR) continue;
            if(abs(qGreen(current) - qGreen(startColor)) > toleranceG) continue;
            if(abs(qBlue(current) - qBlue(startColor)) > toleranceB) continue;
            if(abs(qAlpha(current) - qAlpha(startColor)) > toleranceA) continue;

            image.setPixel(p, fillColor);
            queue.push(p + QPoint(-1, 0));
            queue.push(p + QPoint(1, 0));
            queue.push(p + QPoint(0, -1));
            queue.push(p + QPoint(0, 1));
        }

    }
}

void FillColorTool::valueChanged(int value)
{
    this->spinbox_tolerance_red->setValue(this->slider_tolerance_all->value());
    this->spinbox_tolerance_green->setValue(this->slider_tolerance_all->value());
    this->spinbox_tolerance_blue->setValue(this->slider_tolerance_all->value());
    this->spinbox_tolerance_alpha->setValue(this->slider_tolerance_all->value());
}
