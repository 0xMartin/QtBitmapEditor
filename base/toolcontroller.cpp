#include "toolcontroller.h"

ToolController::ToolController(QWidget *parent) : QWidget(parent)
{
    this->tool = NULL;
    this->project = NULL;
    this->mainLayout = new QHBoxLayout(this);
    this->setLayout(this->mainLayout);
}

ToolController::~ToolController()
{
    if(this->mainLayout) delete this->mainLayout;
}

void ToolController::setProject(Project *project)
{
    this->project = project;
}

Project *ToolController::getProject() const
{
    return this->project;
}

Tool *ToolController::getTool() const
{
    return tool;
}

void ToolController::setTool(Tool *newTool)
{
    if (tool == newTool)
        return;
    tool = newTool;
    emit toolChanged();
}
