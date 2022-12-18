#include "text.h"

#include <QVBoxLayout>
#include <QPainterPath>

#include "../base/config.h"
#include "../layer/textlayer.h"


TextTool::TextTool(QObject *parent) : Tool(parent)
{
    this->name = tr("TEXT");

    //****************************************************************************************
    // sestaveni UI pro ovladani nastroje
    //****************************************************************************************
    this->ui = new QWidget();
    this->layout = new QVBoxLayout(this->ui);
    this->ui->setLayout(this->layout);

    // text
    this->lineEdit_text = new QLineEdit(this->ui);
    this->layout->addWidget(this->lineEdit_text);

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


    // events
    connect(this->lineEdit_text, SIGNAL(textChanged(QString)),
            this, SLOT(on_text_textChanged(QString)));
    connect(this->colorPicker, SIGNAL(colorChange(QColor)),
            this, SLOT(on_colorPicker_colorChange(QColor)));
    connect(this->fontSelector, SIGNAL(fontChanged(QFont)),
            this, SLOT(on_fontSelector_fontChanged(QFont)));
    connect(this->spinbox_x, SIGNAL(valueChanged(int)),
            this, SLOT(on_spinbox_x_valueChanged(int)));
    connect(this->spinbox_y, SIGNAL(valueChanged(int)),
            this, SLOT(on_spinbox_y_valueChanged(int)));

    // spacer
    this->layout->addSpacerItem(
                new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding));
}

TextTool::~TextTool()
{
    if(this->layout) delete this->layout;
    if(this->lineEdit_text) delete this->lineEdit_text;
    if(this->colorPicker) delete this->colorPicker;
    if(this->fontSelector) delete this->fontSelector;
    if(this->spinbox_x) delete this->spinbox_x;
    if(this->spinbox_y) delete this->spinbox_y;
    if(this->checkBox_Antialiasing) delete this->checkBox_Antialiasing;
}

void TextTool::updatTool(float scale)
{
    TextLayer *layer = (TextLayer*) this->layerCheck(TEXT_LAYER_TYPE);
    if(layer != NULL) {
        this->lineEdit_text->setText(layer->getText());
        this->colorPicker->setColor(layer->getColor());
        this->fontSelector->setSelectedFont(layer->getFont());
        this->spinbox_x->setValue(layer->getPosition().x());
        this->spinbox_y->setValue(layer->getPosition().y());
    }
}

void TextTool::paintEvent(const QPointF &pos, float scale, QPainter &painter)
{
}

bool TextTool::overLayerPainting() const
{
    return false;
}

int TextTool::getType() const
{
    return TOOL_TEXT;
}


/*****************************************************************************************/
// EVENTY PRO EDITACI TEXTU
/*****************************************************************************************/

void TextTool::mousePressEvent(const QPointF &pos)
{
    TextLayer *layer = (TextLayer*) this->layerCheck(TEXT_LAYER_TYPE);
    if(layer != NULL) {
        layer->setPosition(QPoint(pos.x(), pos.y()));
    }
}

void TextTool::mouseReleaseEvent(const QPointF &pos)
{
}

void TextTool::mouseDoubleClickEvent(const QPointF &pos)
{
    // prida novou textovou vrstvu do projektu
    if(this->project != NULL) {
        TextLayer *layer = new TextLayer(
                    this->project,
                    "Text " + QString::number(this->project->getLayers()->size() + 1),
                    "Text",
                    QPoint(pos.x(), pos.y()));
        // prida novou textovou vrstvu a ozaci ji
        this->project->insertLayerAbove(layer);
        this->project->setSelectedLayer(layer);
        emit this->project->layerListChanged();
        // update
        this->updatTool(1.0);
    }
}

void TextTool::mouseMoveEvent(const QPointF &pos)
{
}

void TextTool::outOfAreaEvent(const QPointF &pos)
{
}

void TextTool::on_text_textChanged(const QString &text)
{
    TextLayer *layer = (TextLayer*) this->layerCheck(TEXT_LAYER_TYPE);
    if(layer != NULL) {
        layer->setText(text);
    }
    if(this->project != NULL) {
        this->project->requestRepaint();
    }
}

void TextTool::on_colorPicker_colorChange(const QColor &color)
{
    TextLayer *layer = (TextLayer*) this->layerCheck(TEXT_LAYER_TYPE);
    if(layer != NULL) {
        layer->setColor(color);
    }
    if(this->project != NULL) {
        this->project->requestRepaint();
    }
}

void TextTool::on_spinbox_x_valueChanged(int val)
{
    TextLayer *layer = (TextLayer*) this->layerCheck(TEXT_LAYER_TYPE);
    if(layer != NULL) {
        QPoint p = layer->getPosition();
        p.setX(val);
        layer->setPosition(p);
    }
    if(this->project != NULL) {
        this->project->requestRepaint();
    }
}

void TextTool::on_spinbox_y_valueChanged(int val)
{
    TextLayer *layer = (TextLayer*) this->layerCheck(TEXT_LAYER_TYPE);
    if(layer != NULL) {
        QPoint p = layer->getPosition();
        p.setY(val);
        layer->setPosition(p);
    }
    if(this->project != NULL) {
        this->project->requestRepaint();
    }
}

void TextTool::on_fontSelector_fontChanged(const QFont &font)
{
    TextLayer *layer = (TextLayer*) this->layerCheck(TEXT_LAYER_TYPE);
    if(layer != NULL) {
        layer->setFont(font);
    }
    if(this->project != NULL) {
        this->project->requestRepaint();
    }
}

