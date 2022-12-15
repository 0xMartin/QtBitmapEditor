#include "text.h"

#include <QVBoxLayout>
#include <QPainterPath>

#include "../base/config.h"
#include "../layer/textlayer.h"


Text::Text(QObject *parent, ColorPicker *colorPicker) : Tool(parent)
{
    this->name = tr("TEXT");
    this->mouseHelper = MouseEventHelper(DEFAULT_MOUSE_HELPER_DIST);
    this->colorPicker = colorPicker; 

    //****************************************************************************************
    // sestaveni UI pro ovladani nastroje
    //****************************************************************************************
    this->ui = new QWidget();
    this->layout = new QVBoxLayout(this->ui);
    this->ui->setLayout(this->layout);

    // velikost pera
    this->spinbox_size = new QSpinBox();
    this->spinbox_size->setPrefix(tr("Pen Size:"));
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
    this->layout->addSpacerItem(
                new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding));
}

Text::~Text()
{
    if(this->layout) delete this->layout;
    if(this->spinbox_size) delete this->spinbox_size;
    if(this->checkBox_Antialiasing) delete this->checkBox_Antialiasing;
}

void Text::updatTool(float scale)
{
    // update mouse helper
    this->mouseHelper.updateDistance(mapFunc(scale, 1.0, PIXEL_GRID_MIN_SCALE, DEFAULT_MOUSE_HELPER_DIST, 1.0));

    // update pen
    this->pen = QPen(this->colorPicker->getColor(),
                     this->spinbox_size->value(),
                     Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin);
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
