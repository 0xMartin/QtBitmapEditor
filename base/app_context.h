#ifndef APPCONTEXT_H
#define APPCONTEXT_H


// hlavni komponenty
#include "project.h"
#include "workspace.h"
#include "toolcontroller.h"
#include "layermanager.h"


/**
 * @brief Struktura uchovavajici nejdulezitejsi komponenty aplikace
 */
struct AppContext_t {
    Project *project; /** Aktualne ovevtreni projekt*/
    Workspace *workspace; /** Nejdulezitejsi komponent. Komponenta pro editaci projektu */
    ToolController *toolController; /** Komponenta pro nastavovani parametru aktualne zvoleneho nastroje*/
    LayerManager *layerManager; /** Komponenta pro spravu vrstev projektu*/
};


static void AptCntx_destructAppContext(AppContext_t *context) {
    if(context == NULL) return;
    if(context->project) delete context->project;
    if(context->workspace) delete context->workspace;
    if(context->toolController) delete context->toolController;
    if(context->layerManager) delete context->layerManager;
}

static void AptCntx_setProject(AppContext_t *context, Project * project) {
    if(context == NULL) return;
    context->project = project;
    context->workspace->setProject(project);
    context->layerManager->setProject(project);
}

#endif // APPCONTEXT_H
