#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QWidget>
#include <vector>
#include <QMouseEvent>
#include <QScrollArea>


#include "project.h"
#include "tool.h"
#include "../utility/mouseeventhelper.h"

struct Config_Workspace_t {
    QFont font; /** Font pro texty ve workspace (meritka + pozicni informace)*/
    float mouseSensitivity; /** Citlivost mysi */
    int fps; /** Maximalni rychlost vykreslovani*/
};

/**
 * @brief Tato komponenta se stara o spravne vykreslovani projektu (obrazku) a
 * grafickou manipulaci s nim.
 */
class Workspace : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Vytvori instanci workspac widgetu
     * @param config - Konfiguracni soubor
     * @param parent - Parent widget
     */
    explicit Workspace(const Config_Workspace_t &config, QWidget *parent = nullptr);

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

    // events
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;

    Tool *getTool() const;
    void setTool(Tool *newTool);

    const Config_Workspace_t &getConfig() const;
    void setConfig(const Config_Workspace_t &newConfig);

signals:
    void toolChanged();

    void configChanged();

protected:
    Config_Workspace_t config;

    // aktualni projekt
    Project *project;

    // merito zobrazeni
    float scale;

    // globalni offset
    QPoint globalOffset;

    // aktualni pozice kurzoru
    QPoint currentPos;

    // helper pro mouse eventy
    MouseEventHelper mouseHelper;

    // aktualne zvoleny graficky nastroj
    Tool *tool;

    /**
     * @brief QWidget paint event
     * @param event - QPaintEvent
     */
    void paintEvent(QPaintEvent *event) override;

private:
    // promena pro casovani
    std::chrono::steady_clock::time_point begin;

    /**
     * @brief Vypocita pozici pro eventy projektu (pozici kurzoru prevede na pozici
     * odpovidajici primo na souradnice v projektu)
     * @param pos - Aktualni pozice kurzoru
     * @param outOfRange - Vystupni parametr (bude nastaven na true pokud kurzor bude mymo kreslici oblast)
     * @return QPoint
     */
    QPoint calculateEventOffsetPosition(const QPoint &pos, bool &outOfRange) const;

    Q_PROPERTY(Tool *tool READ getTool WRITE setTool NOTIFY toolChanged)
    Q_PROPERTY(Config_Workspace_t config READ getConfig WRITE setConfig NOTIFY configChanged)
};

/**
 * @brief Vygeneruje defaultni konfiguraci pro workspace
 * @return Config_Workspace_t
 */
Q_DECL_EXPORT Config_Workspace_t Workspace_defaultConfig();

#endif // WORKSPACE_H
