#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include <QObject>
#include <QList>
#include <QDebug>

#include "project.h"
#include "workspace.h"
#include "toolcontroller.h"
#include "layermanager.h"
#include "tool.h"


/**
 * @brief Trida uchovavajici hlavni komponenty aplikace a umoznuje jejich spravu
 */
class AppContext : public QObject {
    Q_OBJECT
public:
    /**
     * @brief Vytvori kontext aplikace
     */
    AppContext();
    ~AppContext();

    /**
     * @brief Nastavi novy projekt (otevre)
     * @param project - Novy projekt
     */
    void setProject(Project * project);

    /**
     * @brief Nastavi novy vybrany graficky nastroj
     * @param tool - Novy graficky nastroj
     */
    void setTool(Tool *tool);

    /**
     * @brief Navrati aktualne otevreny projekt
     * @return Project
     */
    Project *getProject() const;

    /**
     * @brief Navrati aktualne vybrany nastroj
     * @return Tool
     */
    Tool *getTool() const;

    /**
     * @brief Navrati pointer na workspace komponent
     * @return Workspace
     */
    Workspace *getWorkspace() const;

    /**
     * @brief Aplikaci nastavi novy Workspace
     * @param newWorkspace - Novy Workspace
     */
    void setWorkspace(Workspace *newWorkspace);

    /**
     * @brief Navrati pointer na toolcontroller komponentu
     * @return ToolController
     */
    ToolController *getToolController() const;

    /**
     * @brief Aplikaci nastavi novy ToolController
     * @param newToolController - Novy ToolController
     */
    void setToolController(ToolController *newToolController);

    /**
     * @brief Navrati pointer na LayerManager komponentu
     * @return LayerManager
     */
    LayerManager *getLayerManager() const;

    /**
     * @brief Aplikaci nastavi novy LayerManager
     * @param newLayerManager - Novy LayerManager
     * @return
     */
    void setLayerManager(LayerManager *newLayerManager);

    /**
     * @brief Prida do listu novy nastraj
     * @param tool - Novy nastroj
     * @return True -> nastroj uspesne pridan
     */
    bool addTool(Tool *tool);

    /**
     * @brief Vybere nastroj ktery se nachazi v listu nastroju v kontextu aplikace
     * @param toolType - ID typu nastroje
     * @return True -> nastroj uspesne nastaven jako aktivni
     */
    bool selectToolFromList(int toolType);

signals:
    void workspaceChanged();

    void toolControllerChanged();

    void layerManagerChanged();

protected:
    Project *project; /** Aktualne ovevtreni projekt*/
    QList<Tool*> tools; /** List nastroju */
    Tool *tool; /** Aktualne pouzivany graficky nastroj */
    Workspace *workspace; /** Nejdulezitejsi komponent. Komponenta pro editaci projektu */
    ToolController *toolController; /** Komponenta pro nastavovani parametru aktualne zvoleneho nastroje*/
    LayerManager *layerManager; /** Komponenta pro spravu vrstev projektu*/

private:
    Q_PROPERTY(Workspace *workspace READ getWorkspace WRITE setWorkspace NOTIFY workspaceChanged)
    Q_PROPERTY(ToolController *toolController READ getToolController WRITE setToolController NOTIFY toolControllerChanged)
    Q_PROPERTY(LayerManager *layerManager READ getLayerManager WRITE setLayerManager NOTIFY layerManagerChanged)
};

#endif // APPCONTEXT_H
