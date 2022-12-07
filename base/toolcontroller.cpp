#include "toolcontroller.h"

ToolController::ToolController(QWidget *parent) : QWidget(parent)
{
    this->tool = NULL;
    this->project = NULL;
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
    return this->tool;
}

void ToolController::setTool(Tool *newTool)
{
    if (this->tool == newTool)
        return;
    this->tool = newTool;
    emit toolChanged();

    if(this->tool != NULL) {
        this->setLayout(this->tool->getUI());
    }
}
