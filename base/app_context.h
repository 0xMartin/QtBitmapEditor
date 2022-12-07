#ifndef APPCONTEXT_H
#define APPCONTEXT_H

#include <QObject>

#include "project.h"
#include "workspace.h"
#include "toolcontroller.h"
#include "layermanager.h"
#include "tool.h"


/**
 * @brief The AppContext class
 */
class AppContext : public QObject {
    Q_OBJECT
public:
    /**
     * @brief AppContext
     */
    AppContext();
    ~AppContext();

    /**
     * @brief setProject
     * @param project
     */
    void setProject(Project * project);

    /**
     * @brief setTool
     * @param tool
     */
    void setTool(Tool *tool);

    /**
     * @brief getProject
     * @return
     */
    Project *getProject() const;

    /**
     * @brief getTool
     * @return
     */
    Tool *getTool() const;

    /**
     * @brief getWorkspace
     * @return
     */
    Workspace *getWorkspace() const;

    /**
     * @brief setWorkspace
     * @param newWorkspace
     */
    void setWorkspace(Workspace *newWorkspace);

    /**
     * @brief getToolController
     * @return
     */
    ToolController *getToolController() const;

    /**
     * @brief setToolController
     * @param newToolController
     */
    void setToolController(ToolController *newToolController);

    /**
     * @brief getLayerManager
     * @return
     */
    LayerManager *getLayerManager() const;

    /**
     * @brief setLayerManager
     * @param newLayerManager
     */
    void setLayerManager(LayerManager *newLayerManager);

signals:
    void workspaceChanged();

    void toolControllerChanged();

    void layerManagerChanged();

protected:
    Project *project; /** Aktualne ovevtreni projekt*/
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
