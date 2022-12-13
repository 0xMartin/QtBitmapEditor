#include "tool.h"

Tool::Tool(QObject *parent) : QObject(parent)
{
    this->project = NULL;
    this->ui = new QWidget();
    this->name = "";
    this->mouseTracking = false;
}

Tool::~Tool()
{
    if(this->ui) delete this->ui;
}

QWidget *Tool::getUI() const
{
    return this->ui;
}

Project *Tool::getProject() const
{
    return project;
}

void Tool::setProject(Project *newProject)
{
    if (this->project == newProject)
        return;
    this->project = newProject;
    emit projectChanged();
}

bool Tool::overLayerPainting() const
{
    return false;
}

const QString &Tool::getName() const
{
    return this->name;
}

void Tool::setMouseTracking(bool mouseTracking)
{
    this->mouseTracking = mouseTracking;
}

bool Tool::isMouseTrackingEnabled() const
{
    return this->mouseTracking;
}

Layer *Tool::layerCheck(int type)
{
    if(this->project == NULL) return NULL;
    Layer *l = this->project->getSelectedLayer();
    if(l != NULL && type >= 0) {
        if(l->getType() != type) return NULL;
        if(!l->isVisible()) return NULL;
    }
    return l;
}
