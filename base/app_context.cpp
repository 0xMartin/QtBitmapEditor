#include "app_context.h"


AppContext::AppContext()
{
    this->project = NULL;
    this->tool = NULL;
    this->workspace = NULL;
    this->layerManager = NULL;
    this->toolController = NULL;

    this->copymask = NULL;
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
    if(project == NULL) return;

    if(this->project) delete this->project;
    this->project = project;

    if(this->tool) this->tool->setProject(project);
    if(this->workspace) this->workspace->setProject(project);
    if(this->toolController) this->toolController->setProject(project);
    if(this->layerManager) this->layerManager->setProject(project);
}

void AppContext::setTool(Tool *tool)
{
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
    this->workspace->setContext(this);
    this->workspace->setProject(this->project);
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
    this->toolController->setContext(this);
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
    this->layerManager->setContext(this);
    this->layerManager->setProject(this->project);
    emit layerManagerChanged();
}

bool AppContext::addTool(Tool *tool)
{
    if(tool!= NULL) {
        for(Tool *t: this->tools) {
            if(t != NULL) {
                if(t->getType() == tool->getType()) {
                    return false;
                }
            }
        }
        this->tools.push_back(tool);
    }
    return true;
}

bool AppContext::selectToolFromList(int toolType)
{
    for(Tool *tool: this->tools) {
        if(tool != NULL) {
            if(tool->getType() == toolType) {
                this->setTool(tool);
                return true;
            }
        }
    }
    return false;
}

void AppContext::copyMask(QBitmap *mask)
{
    if(mask == NULL) return;
    if(this->copymask) delete this->copymask;
    this->copymask = new QBitmap(mask->size());
    *this->copymask = *mask;
}

QBitmap *AppContext::getMaskCopy() const
{
    return this->copymask;
}

