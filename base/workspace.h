#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QWidget>
#include <vector>
#include <QMouseEvent>
#include <QScrollArea>

#include "module.h"
#include "project.h"
#include "tool.h"
#include "../utility/mouseeventhelper.h"


/**
 * @brief Konfiguracn struktura pro workspace
 */
struct Config_Workspace_t {
    QFont font; /** Font pro texty ve workspace (meritka + pozicni informace)*/
    float mouseSensitivity; /** Citlivost mysi */
    int fps; /** Maximalni rychlost vykreslovani*/
    QColor maskColor; /** Barva masky v editacnim rezimu (moznost prizpusobeni pro lepsi praci) */
};


/**
 * @brief Tato komponenta se stara o spravne vykreslovani projektu (obrazku) a
 * grafickou manipulaci s nim.
 */
class Workspace : public QWidget, public Module
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
     * @brief Priblizi editovany projekt o definovany krok
     */
    void zoomIN();

    /**
     * @brief Oddali editovany projekt o definovany krok
     */
    void zoomOUT();

    /**
     * @brief Navrati meritko
     * @return meritko
     */
    float getScale() const;

    /**
     * @brief Navrati aktualne vybrany nastroj
     * @return Tool
     */
    Tool *getTool() const;

    /**
     * @brief Nastavy novy nastroj
     * @param newTool - Novy nastroj
     */
    void setTool(Tool *newTool);

    /**
     * @brief Navrati konfiguraci workspacu
     * @return Config_Workspace_t
     */
    const Config_Workspace_t &getConfig() const;

    /**
     * @brief Nastavi novou konfiguraci
     * @param newConfig - Config_Workspace_t
     */
    void setConfig(const Config_Workspace_t &newConfig);

    // events
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;

signals:
    /**
     * @brief Signal vyvolan ve chvili zmeny nastroje
     */
    void toolChanged();

    /**
     * @brief Signal vyvolan ve chvili zmeny konfiguracni struktury
     */
    void configChanged();

protected:
    // konfigurace
    Config_Workspace_t config;

    // aktualni projekt
    Project *project;

    // merito zobrazeni
    float scale;

    // globalni offset (pro pohybovani projektem)
    QPointF globalOffset;

    // aktualni pozice kurzoru (pro press a current)
    QPointF pressPos;
    QPointF currentPos;

    // helper pro mouse eventy
    MouseEventHelper mouseHelper;

    // aktualne zvoleny graficky nastroj (manipuluje z projektem)
    Tool *tool;

    /**
     * @brief [[[ Hlavni paint event ]]]
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
    QPointF calculateEventOffsetPosition(const QPointF &pos, bool &outOfRange) const;

    Q_PROPERTY(Tool *tool READ getTool WRITE setTool NOTIFY toolChanged)
    Q_PROPERTY(Config_Workspace_t config READ getConfig WRITE setConfig NOTIFY configChanged)

public slots:
    /**
     * @brief Zobrazi kontextove menu workspacu
     * @param pos - Pozice
     */
    void showContextMenu(const QPoint &pos);

    /**
     * @brief Zmeni barvu masky (jen pro usnadneni pri praci "zabraneni splynuti barev")
     */
    void changeMaskColor();

    /**
     * @brief Vymaze masku (vse ve vrstve bude viditelny)
     */
    void clearMask();

    /**
     * @brief Nastavi masku (nic ve vrstve nebude viditelne)
     */
    void setMask();

    /**
     * @brief Zkopiruje masku (masku muze vlozit do jine libovolne masky / pri
     *  vytvareni dalsi masky se automaticky prekopiruje do nove vznikle masky)
     */
    void maskCopy();

    /**
     * @brief Vlozi zkopirovanou masku do masky aktualne vybrane vrstvy
     */
    void maskPaste();

};

/**
 * @brief Vygeneruje defaultni konfiguraci pro workspace
 * @return Config_Workspace_t
 */
Q_DECL_EXPORT Config_Workspace_t Workspace_defaultConfig();

#endif // WORKSPACE_H
