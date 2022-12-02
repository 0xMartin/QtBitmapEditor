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
    Workspace * workspace; /** Nejdulezitejsi komponent. Zde se zobrazuje obrazek*/
    ToolController * toolController; /** Komponenta pro nastavovani parametru aktualne zvoleneho nastroje*/
    LayerManager * layerManager; /** Koponenta pro spravu vrstev projektu*/
};


static void destructAppContext(AppContext_t &context) {
    if(context.workspace) delete context.workspace;
    if(context.toolController) delete context.toolController;
    if(context.layerManager) delete context.layerManager;
}

#endif // APPCONTEXT_H
