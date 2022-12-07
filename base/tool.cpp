#include "tool.h"

Tool::Tool(QObject *parent) : QObject(parent)
{
    this->ui = NULL;
    this->layer = NULL;
}

Tool::~Tool()
{
}

void Tool::bindLayer(Layer *layer)
{
    this->layer = layer;
}

QLayout *Tool::getUI()
{
    return this->ui;
}
