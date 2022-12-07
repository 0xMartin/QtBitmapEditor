#include "tool.h"

Tool::Tool(QObject *parent) : QObject(parent)
{
    this->ui = NULL;
    this->project = NULL;
}

Tool::~Tool()
{
}

QLayout *Tool::getUI()
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

Layer *Tool::layerCheck(int type)
{
    if(this->project == NULL) return NULL;
    Layer *l = this->project->getSelectedLayer();
    if(l != NULL && type >= 0) {
        if(l->getType() != type) return NULL;
    }
    return l;
}
