#ifndef TOOLCONTROLLER_H
#define TOOLCONTROLLER_H

#include <QWidget>
#include <QHBoxLayout>
#include <QList>
#include <QLabel>

#include "tool.h"
#include "project.h"

/**
 * @brief Komponenta tool kontroler umoznuje nastavovat parametry zvoleneho nastroje
 */
class ToolController : public QWidget
{
    Q_OBJECT

public:
    explicit ToolController(QWidget *parent = nullptr);
    ~ToolController();

    /**
     * @brief Nastavi projekt se ktery se bude pracovat
     * @param project - Pointer na projekt
     */
    void setProject(Project *project);

    /**
     * @brief Navrati pointer na projekt
     * @return Pointer na projekt
     */
    Project *getProject() const;

    /**
     * @brief  Navrati aktualne zvoleny nastroj
     * @return Tool
     */
    Tool *getTool() const;

    /**
     * @brief Nastavy novy nastroj
     * @param newTool - Novy nastroj
     */
    void setTool(Tool *newTool);

signals:
    void toolChanged();

protected:
    // aktualne vybrany nastroj
    Tool *tool;

    // aktualni projekt
    Project *project;

    QLabel *header;
    QVBoxLayout *layout;

private:
    Q_PROPERTY(Tool *tool READ getTool WRITE setTool NOTIFY toolChanged)
};

#endif // TOOLCONTROLLER_H
