#include "text.h"

#include <QVBoxLayout>
#include <QPainterPath>

#include "../base/config.h"
#include "../layer/textlayer.h"


Text::Text(QObject *parent) : Tool(parent)
{
    this->name = tr("TEXT");
    this->mouseHelper = MouseEventHelper(DEFAULT_MOUSE_HELPER_DIST);

    //****************************************************************************************
    // sestaveni UI pro ovladani nastroje
    //****************************************************************************************
    this->ui = new QWidget();
    this->layout = new QVBoxLayout(this->ui);
    this->ui->setLayout(this->layout);

    // barva textu
    this->colorPicker = new ColorPicker(this->ui);
    this->layout->addWidget(this->colorPicker);

    // font
    this->fontSelector = new FontSelector(this->ui);
    this->layout->addWidget(this->fontSelector);

    // x pozice
    this->spinbox_x = new QSpinBox();
    this->spinbox_x->setPrefix(tr("X:"));
    this->spinbox_x->setSuffix("px");
    this->spinbox_x->setMinimum(POSITION_MIN);
    this->spinbox_x->setMaximum(POSITION_MAX);
    this->spinbox_x->setValue(DEFAULT_TOOL_SIZE);
    this->layout->addWidget(this->spinbox_x);

    // y pozice
    this->spinbox_y = new QSpinBox();
    this->spinbox_y->setPrefix(tr("Y:"));
    this->spinbox_y->setSuffix("px");
    this->spinbox_y->setMinimum(POSITION_MIN);
    this->spinbox_y->setMaximum(POSITION_MAX);
    this->spinbox_y->setValue(DEFAULT_TOOL_SIZE);
    this->layout->addWidget(this->spinbox_y);

    // antialiasing
    this->checkBox_Antialiasing = new QCheckBox();
    this->checkBox_Antialiasing->setChecked(true);
    this->checkBox_Antialiasing->setText(tr("Smooth Painting Mode"));
    this->layout->addWidget(this->checkBox_Antialiasing);

    // spacer
    this->layout->addSpacerItem(
                new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding));
}

Text::~Text()
{
    if(this->layout) delete this->layout;
    if(this->colorPicker) delete this->colorPicker;
    if(this->fontSelector) delete this->fontSelector;
    if(this->spinbox_x) delete this->spinbox_x;
    if(this->spinbox_y) delete this->spinbox_y;
    if(this->checkBox_Antialiasing) delete this->checkBox_Antialiasing;
}

void Text::updatTool(float scale)
{
    // update mouse helper
    this->mouseHelper.updateDistance(mapFunc(scale, 1.0, PIXEL_GRID_MIN_SCALE, DEFAULT_MOUSE_HELPER_DIST, 1.0));
}

void Text::paintEvent(const QPointF &pos, float scale, QPainter &painter)
{

}

bool Text::overLayerPainting() const
{
    return false;
}

int Text::getType() const
{
    return TOOL_TEXT;
}


/*****************************************************************************************/
// EVENTY PRO EDITACI TEXTU
/*****************************************************************************************/

void Text::mousePressEvent(const QPointF &pos)
{
    this->mouseHelper.processMoveEvent(pos);
    // prida novou textovou vrstvu do projektu
    if(this->project != NULL) {
        TextLayer *layer = new TextLayer(
                    this->project,
                    "Text " + QString::number(this->project->getLayers()->size() + 1),
                    "Text",
                    QPoint(pos.x(), pos.y()));
        qDebug() << "AAA";
        this->project->insertLayerAbove(layer);
    }

}

void Text::mouseReleaseEvent(const QPointF &pos)
{
    this->mouseHelper.resetMove();
}

void Text::mouseDoubleClickEvent(const QPointF &pos)
{

}

void Text::mouseMoveEvent(const QPointF &pos)
{

}

void Text::outOfAreaEvent(const QPointF &pos)
{

}
