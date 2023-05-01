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
    this->layout->addWidget(this->header);

    this->scroll = new QScrollArea(this);
    this->scroll->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
    this->scroll->setWidgetResizable(true);
    this->layout->addWidget(this->scroll);
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

    // zmeni nastroj
    this->tool = newTool;
    emit toolChanged();

    // zmena UI
    if(this->tool != NULL) {
        // zmena headeru
        this->header->setText("<b>TOOL</b> " + this->tool->getName());
        // nejdriv odebere nastroj (zabrani jeho odstraneni z pameti)
        this->scroll->takeWidget();
        // nastavi UI noveho nastroje
        this->scroll->setWidget(this->tool->getUI());
        this->repaint();
    }
}
