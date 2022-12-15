#include "textlayer.h"

TextLayer::TextLayer(QObject *parent, const QString &name, const QString &text) : Layer(parent, name)
{
    this->text = text;
    this->font.setFamily("Times");
    this->font.setPixelSize(40);
    this->font.setStyle(QFont::StyleNormal);
}

void TextLayer::paintEvent(QPainter &painter)
{
    painter.setPen(this->color);
    painter.setFont(this->font);
    painter.drawText(this->position, this->text);
}

int TextLayer::getType()
{
    return TEXT_LAYER_TYPE;
}

Layer *TextLayer::createDuplicate() const
{
    // vytvoreni duplikatu
    TextLayer *layer = new TextLayer(this->parent(), this->name, this->text);
    layer->mask = this->duplicateMask();
    layer->maskActive = this->maskActive;
    layer->opacity = this->opacity;
    layer->blendMode = this->blendMode;
    layer->antialiasing = this->antialiasing;
    // #############################
    layer->font = this->font;
    layer->color = this->color;
    layer->position = this->position;
    // #############################
    return layer;
}
