#ifndef APPCONTEXT_H
#define APPCONTEXT_H


// hlavni komponenty
#include "component/workspace.h"
#include "component/toolcontroller.h"
#include "component/layermanager.h"


/**
 * @brief Struktura uchovavajici nejdulezitejsi komponenty aplikace
 */
struct AppContext_t {
    Project *project; /** Aktualne ovevtreni projekt*/
    Workspace *workspace; /** Nejdulezitejsi komponent. Zde se zobrazuje obrazek*/
    ToolController *toolController; /** Komponenta pro nastavovani parametru aktualne zvoleneho nastroje*/
    LayerManager *layerManager; /** Koponenta pro spravu vrstev projektu*/
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
