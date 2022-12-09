#include "toolcontroller.h"

#include <QLayoutItem>


ToolController::ToolController(QWidget *parent) : QWidget(parent)
{
    this->tool = NULL;
    this->project = NULL;
    this->layout = new QVBoxLayout();
    this->layout->setSpacing(0);
    this->setLayout(layout);

    this->header = new QLabel(this);
    this->header->setText("Tool: ");
    this->header->setStyleSheet("background: rgb(41, 41, 41); color: rgb(224, 224, 224); padding: 8px; border-bottom: 1px solid black; border-radius: 4px;");
}

ToolController::~ToolController()
{
    if(this->layout) delete layout;
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
        // clear
        QLayoutItem *item;
        while ((item = this->layout->takeAt(0)) != 0) {
            if(item->widget())
                item->widget()->setParent(NULL);
        }

        // znovu sestaveni
        this->header->setText("<b>TOOL</b> " + newTool->getName());
        this->layout->addWidget(this->header);
        this->layout->addWidget(this->tool->getUI());
        this->repaint();
    }
}
