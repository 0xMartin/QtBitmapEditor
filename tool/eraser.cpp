#include "eraser.h"

#include <QVBoxLayout>
#include <QPainterPath>
#include <QMessageBox>

#include "../base/config.h"
#include "../layer/bitmaplayer.h"


EraserTool::EraserTool(QObject *parent) : Tool(parent)
{
    this->name = tr("ERASER");
    this->mouseHelper = MouseEventHelper(DEFAULT_MOUSE_HELPER_DIST);

    //****************************************************************************************
    // sestaveni UI pro ovladani nastroje
    //****************************************************************************************
    this->ui = new QWidget();
    this->layout = new QVBoxLayout(this->ui);
    this->ui->setLayout(this->layout);

    // velikost pera
    this->spinbox_size = new QSpinBox();
    this->spinbox_size->setPrefix(tr("Eraser Size:"));
    this->spinbox_size->setSuffix("px");
    this->spinbox_size->setMinimum(MIN_TOOL_SIZE);
    this->spinbox_size->setMaximum(MAX_TOOL_SIZE);
    this->spinbox_size->setValue(DEFAULT_TOOL_SIZE);
    this->layout->addWidget(this->spinbox_size);

    // antialiasing
    this->checkBox_Antialiasing = new QCheckBox();
    this->checkBox_Antialiasing->setChecked(true);
    this->checkBox_Antialiasing->setText(tr("Smooth Painting Mode"));
    this->layout->addWidget(this->checkBox_Antialiasing);

    // spacer
    this->layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding));
}

EraserTool::~EraserTool()
{
    if(this->layout) delete this->layout;
    if(this->spinbox_size) delete this->spinbox_size;
    if(this->checkBox_Antialiasing) delete this->checkBox_Antialiasing;
}

void EraserTool::updatTool(float scale)
{
    // update mouse helper
    this->mouseHelper.updateDistance(mapFunc(scale, 1.0, PIXEL_GRID_MIN_SCALE, DEFAULT_MOUSE_HELPER_DIST, 1.0));

    // update pen
    this->pen = QPen(Qt::transparent,
                     this->spinbox_size->value(),
                     Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
}

void EraserTool::paintEvent(const QPointF &pos, float scale, QPainter &painter)
{
    // vykresleni tvaru a veliskota nastroje do horni nahledove vrstvy
    int size = this->spinbox_size->value();
    painter.setPen(Qt::black);
    float s = size * scale;
    painter.drawEllipse(pos.x() - s/2, pos.y() - s/2, s, s);
}

bool EraserTool::overLayerPainting() const
{
    return true;
}

int EraserTool::getType() const
{
    return TOOL_ERASER;
}


/*****************************************************************************************/
// EVENTY PRO EDITACI BITMAPY
/*****************************************************************************************/

void EraserTool::mousePressEvent(const QPointF &pos)
{
    this->mouseHelper.processMoveEvent(pos);

    // vykreleni po dotiku
    int size = this->spinbox_size->value();
    BitmapLayer *layer = (BitmapLayer *)this->layerCheck(BITMAP_LAYER_TYPE);
    if(layer == NULL) {
        QMessageBox::warning(
                    this->ui,
                    tr("Eraser Tool"),
                    tr("The selected layer is not a bitmap format! It must be converted to bitmap format."));
        return;
    }

    switch(this->project->getMode()) {
    case PROJECT_EDIT:
        this->painter.begin(&layer->image);
        break;
    case MASK_EDIT:
        if(layer->getMask() == NULL) return;
        this->painter.begin(layer->getMask());
        break;
    }

    painter.setRenderHint(QPainter::Antialiasing, this->checkBox_Antialiasing->isChecked());
    this->painter.setBrush(Qt::transparent);
    this->painter.setCompositionMode(QPainter::CompositionMode_Clear);
    this->painter.drawEllipse(pos.x() - size/2, pos.y() - size/2, size, size);
    this->painter.end();
}

void EraserTool::mouseReleaseEvent(const QPointF &pos)
{
    this->mouseHelper.resetMove();
}

void EraserTool::mouseDoubleClickEvent(const QPointF &pos)
{

}

void EraserTool::mouseMoveEvent(const QPointF &pos)
{
    if(this->mouseHelper.processMoveEvent(pos)) {
        // po definovanych vzdalenost dela tah
        QLineF line = this->mouseHelper.lineFromLastPos();

        BitmapLayer *layer = (BitmapLayer *)this->layerCheck(BITMAP_LAYER_TYPE);
        if(layer == NULL) return;
        switch(this->project->getMode()) {
        case PROJECT_EDIT:
            this->painter.begin(&layer->image);
            break;
        case MASK_EDIT:
            if(layer->getMask() == NULL) return;
            this->painter.begin(layer->getMask());
            break;
        }

        painter.setRenderHint(QPainter::Antialiasing, this->checkBox_Antialiasing->isChecked());
        this->painter.setPen(this->pen);
        this->painter.setCompositionMode(QPainter::CompositionMode_Clear);
        this->painter.drawLine(line);
        this->painter.end();
    }
}

void EraserTool::outOfAreaEvent(const QPointF &pos)
{
    // dokonci tah
    const QPointF *last = this->mouseHelper.getLast();
    if(last) {
        QLineF line(*last, pos);

        BitmapLayer *layer = (BitmapLayer *)this->layerCheck(BITMAP_LAYER_TYPE);
        if(layer == NULL) return;
        switch(this->project->getMode()) {
        case PROJECT_EDIT:
            this->painter.begin(&layer->image);
            break;
        case MASK_EDIT:
            if(layer->getMask() == NULL) return;
            this->painter.begin(layer->getMask());
            break;
        }

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
