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
     * automaticky odstranen z pameti pri destrukci teto tridy.
     * @param project - Pointer na projekt
     */
    void setProject(Project *project);

    /**
     * @brief Navrati pointer na projekt
     * @return Pointer na projekt
     */
    Project *getProject() const;
protected:
    // projekt
    Project *project;

    // QWidget paint event
    void paintEvent(QPaintEvent *event) override;

};

#endif // WORKSPACE_H
