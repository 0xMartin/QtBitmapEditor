#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QWidget>
#include <vector>

#include "../objects/project.h"

/**
 * @brief Tato komponenta se stara o spravne vykreslovani projektu (obrazku) a
 * grafickou manipulaci s nim
 */
class Workspace : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Vytvori workspace
     * @param parent - Parrent objekt
     */
    explicit Workspace(QWidget *parent = nullptr);

    ~Workspace();

    /**
     * @brief Nastavi projekt se ktery se bude pracovat. Projekt bude
     * automaticky odstranendz pameti pri destrukci teto tridy.
     * @param project - Pointer na projekt
     */
    void setProject(Project *project);

    /**
     * @brief Navrati pointer na projekt
     * @return Pointer na projekt
     */
    Project *getProject() const;

    /**
     * @brief Nastavi nove meritko pro zobrazovani obrazku
     * @param scale - Meritko
     */
    void setScale(float scale);

    /**
     * @brief Navrati meritko
     * @return meritko
     */
    float getScale() const;
protected:
    // projekt
    Project *project;

    // merito zobrazeni
    float scale;

    /**
     * @brief QWidget paint event
     * @param event - QPaintEvent
     */
    void paintEvent(QPaintEvent *event) override;

    /**
     * @brief Obnoveni velikosti workspacu (minimalni velikost == velikost parent objektu,
     * aktualni velikost vzdy odpovida tak aby se na workspace vesel cely obrazek i po aplikace scale)
     */
    void updateSizeOfWorkaspace();

};

#endif // WORKSPACE_H
