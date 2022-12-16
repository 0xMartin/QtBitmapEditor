#include "textlayer.h"

TextLayer::TextLayer(QObject *project, const QString &name,
                     const QString &text, const QPoint &pos) : Layer(project, name)
{
    this->text = text;
    this->position = pos;
    this->font.setFamily("Times");
    this->font.setPointSize(40);
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
    TextLayer *layer = new TextLayer(this->parent(), this->name,
                                     this->text, this->position);
    layer->mask = this->duplicateMask();
    layer->maskActive = this->maskActive;
    layer->opacity = this->opacity;
    layer->blendMode = this->blendMode;
    layer->antialiasing = this->antialiasing;
    // #############################
    layer->font = this->font;
    layer->color = this->color;
    // #############################
    return layer;
}

const QString &TextLayer::getText() const
{
    return text;
}

void TextLayer::setText(const QString &newText)
{
    text = newText;
}

const QFont &TextLayer::getFont() const
{
    return font;
}

void TextLayer::setFont(const QFont &newFont)
{
    font = newFont;
}

const QColor &TextLayer::getColor() const
{
    return color;
}

void TextLayer::setColor(const QColor &newColor)
{
    color = newColor;
}

QPoint TextLayer::getPosition() const
{
    return position;
}

void TextLayer::setPosition(QPoint newPosition)
{
    position = newPosition;
}
