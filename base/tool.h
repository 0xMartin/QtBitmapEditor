#ifndef TOOL_H
#define TOOL_H

#include <QObject>
#include <QWidget>
#include <QList>

#include "project.h"
#include "layer.h"

class Tool : public QObject
{
    Q_OBJECT
public:
    Tool(QObject *parent);

    virtual ~Tool();

    /**
     * @brief Navrati UI tohoto nastroje
     * @return QLayout
     */
    QLayout *getUI();

    // events
    virtual void mousePressEvent(const QPoint &pos) = 0;
    virtual void mouseReleaseEvent(const QPoint &pos) = 0;
    virtual void mouseDoubleClickEvent(const QPoint &pos) = 0;
    virtual void mouseMoveEvent(const QPoint &pos) = 0;
    virtual void outOfAreaEvent(const QPoint &pos) = 0;

    /**
     * @brief Navrati aktualne vybrany projekt
     * @return Project
     */
    Project *getProject() const;

    /**
     * @brief Nastavi projekt se kterym bude nastroj manipulovat
     * @param newProject - Projekt
     */
    void setProject(Project *newProject);

signals:
    void projectChanged();

protected:
    // manipulovana vrstva
    Project *project;

    //ovladaci UI nastroje
    QLayout *ui;

    /**
     * @brief Pomocna funkce pro navrace aktualne vybrane vrstvy s konrolou typu vrstvy
     * @param type - ID typu vrstvy. Pokud aktualne vybrana vrstvy neodpovida typu pak
     *  funkce navrati NULL.
     * @return Layer
     */
    Layer *layerCheck(int type);
private:
    Q_PROPERTY(Project *project READ getProject WRITE setProject NOTIFY projectChanged)
};

#endif // TOOL_H
