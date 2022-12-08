#include "tool.h"

Tool::Tool(QObject *parent) : QObject(parent)
{
    this->ui = NULL;
    this->project = NULL;
    this->ui = new QWidget();
    this->name = "";
}

Tool::~Tool()
{
    if(this->ui) delete this->ui;
}

QWidget *Tool::getUI()
{
    return this->ui;
}

Project *Tool::getProject() const
{
    return project;
}

void Tool::setProject(Project *newProject)
{
    if (project == newProject)
        return;
    project = newProject;
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
