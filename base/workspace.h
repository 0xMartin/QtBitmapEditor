#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QWidget>
#include <vector>
#include <QMouseEvent>
#include <QScrollArea>


#include "project.h"
#include "tool.h"
#include "../utility/mouseeventhelper.h"


/**
 * @brief Tato komponenta se stara o spravne vykreslovani projektu (obrazku) a
 * grafickou manipulaci s nim. Tento widget musi byt umisten v QScrollArea!!!
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
     * @brief Nastavi nove meritko pro zobrazovani obrazku
     * @param scale - Meritko
     */
    void setScale(float scale);

    /**
     * @brief Zmeni meritko o urcitou hodnotu
     * @param diff - Zmena meritka
     */
    void addScale(float diff);

    /**
     * @brief Navrati meritko
     * @return meritko
     */
    float getScale() const;

    /**
     * @brief Navrati font (meritak + info)
     * @return QFont
     */
    const QFont &getFont() const;

    /**
     * @brief Nastavi novy font (meritak + info)
     * @param newFont - Novy font
     */
    void setFont(const QFont &newFont);

    // events
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;

    Tool *getTool() const;
    void setTool(Tool *newTool);

signals:
    void toolChanged();

protected:
    // aktualni projekt
    Project *project;

    // merito zobrazeni
    float scale;

    // globalni offset
    QPoint globalOffset;

    // aktualni pozice kurzoru
    QPoint currentPos;

    // font meritek
    QFont font;

    // helper pro mouse eventy
    MouseEventHelper mouseHelper;

    // pointer na parent scroll area component
    QScrollArea *parentScrollArea;

    // aktualne zvoleny graficky nastroj
    Tool *tool;

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

private:
    /**
     * @brief Vypocita pozici pro eventy projektu (pozici kurzoru prevede na pozici
     * odpovidajici primo na souradnice v projektu)
     * @param pos - Aktualni pozice kurzoru
     * @return QPoint
     */
    QPoint calculateEventOffsetPosition(const QPoint &pos) const;

    /**
     * @brief Navrati offset view portu (offset parent scoll area)
     * @return QPoint
     */
    QPoint getViewPortOffset() const;
    Q_PROPERTY(Tool *tool READ getTool WRITE setTool NOTIFY toolChanged)
};

#endif // WORKSPACE_H
