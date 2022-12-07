#include "app_context.h"


AppContext::AppContext()
{
    this->project = NULL;
    this->tool = NULL;
    this->workspace = NULL;
    this->layerManager = NULL;
    this->toolController = NULL;
}

AppContext::~AppContext()
{
    if(this->project) delete this->project;
    if(this->tool) delete this->tool;
    if(this->workspace) delete this->workspace;
    if(this->toolController) delete this->toolController;
    if(this->layerManager) delete this->layerManager;
}

void AppContext::setProject(Project *project)
{
    this->project = project;
    if(this->tool) this->tool->setProject(project);
    if(this->workspace) this->workspace->setProject(project);
    if(this->toolController) this->toolController->setProject(project);
    if(this->layerManager) this->layerManager->setProject(project);
}

void AppContext::setTool(Tool *tool)
{
    if(this->tool) delete this->tool;
    this->tool = tool;
    this->tool->setProject(this->project);
    if(this->workspace) this->workspace->setTool(tool);
    if(this->toolController) this->toolController->setTool(tool);
}

Project *AppContext::getProject() const
{
    return this->project;
}

Tool *AppContext::getTool() const
{
    return this->tool;
}

Workspace *AppContext::getWorkspace() const
{
    return this->workspace;
}

void AppContext::setWorkspace(Workspace *newWorkspace)
{
    if (this->workspace == newWorkspace)
        return;
    this->workspace = newWorkspace;
    emit workspaceChanged();
}

ToolController *AppContext::getToolController() const
{
    return this->toolController;
}

void AppContext::setToolController(ToolController *newToolController)
{
    if (this->toolController == newToolController)
        return;
    this->toolController = newToolController;
    emit toolControllerChanged();
}

LayerManager *AppContext::getLayerManager() const
{
    return this->layerManager;
}

void AppContext::setLayerManager(LayerManager *newLayerManager)
{
    if (this->layerManager == newLayerManager)
        return;
    this->layerManager = newLayerManager;
    emit layerManagerChanged();
}
