#ifndef TOOL_H
#define TOOL_H

#include <QObject>
#include <QWidget>
#include <QList>

#include "config.h"
#include "project.h"
#include "layer.h"

/**
 * @brief Abstraktni trida pro graficky nastroj. Nastroje umoznuji manipulovat s vrstvamy v projektu.
 * Kazdy nastroj zpracovava eventy od workspacu a podle nich vykonava svoji cinost. Nastavovat nastroj
 * je mozne pomoci UI ktere si kazdy nastroje definuje.
 */
class Tool : public QObject
{
    Q_OBJECT
public:
    Tool(QObject *parent);

    virtual ~Tool();

    /**
     * @brief Navrati UI pro ovladani nastroje
     * @return QWidget
     */
    QWidget *getUI() const;

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

    /**
     * @brief Event pro obnoveni nastroje (je volana pri kliknuti do workspacu (PRESS & RELEASE), pri zmene oznacene vrstvy, pri zmene nastroje)
     * !!!!!!JE ZAKAZANO V TETO METODE VOLAT METODU PROJEKTU PRO ZMENU VYBERU OZNACENE VRSTVY!!!!!!
     */
    virtual void updatTool(float scale) = 0;

    /**
     * @brief Vykresli nastroj do overlayer
     * @param pos - Aktualni pozice kurzoru na workspacu
     * @param scale - Meritko
     * @param painter - QPainter
     */
    virtual void paintEvent(const QPointF &pos, float scale, QPainter &painter) = 0;

    /**
     * @brief Urcuje zda nastroj bude vykreslovan do horni vrstvy (nahled nastroje -> napr velikost stetce)
     * @return True -> bude se zobrazovat
     */
    virtual bool overLayerPainting() const;

    /**
     * @brief Navrati ID nastroje
     * @return ID
     */
    virtual int getType() const = 0;

    /**
     * @brief Navrati jmeno nastroje
     * @return Jmeno nastroje
     */
    const QString &getName() const;

    /**
     * @brief Povoli/Zakaze mouse tracking (True -> mouseMoveEvent je volan i pokud neni stisknute tlacitko)
     * @param mouseTracking - Povoli/Zakaze mouse tracking
     */
    void setMouseTracking(bool mouseTracking);

    /**
     * @brief Zjisti zda je pro nastroj povolen mouse tracking
     * @return bool
     */
    bool isMouseTrackingEnabled() const;

    // events
    virtual void mousePressEvent(const QPointF &pos) = 0;
    virtual void mouseReleaseEvent(const QPointF &pos) = 0;
    virtual void mouseDoubleClickEvent(const QPointF &pos) = 0;
    virtual void mouseMoveEvent(const QPointF &pos) = 0;
    virtual void outOfAreaEvent(const QPointF &pos) = 0;


signals:
    void projectChanged();

protected:
    // manipulovana vrstva
    Project *project;

    //ovladaci UI nastroje
    QWidget *ui;

    // jmeno nastoroje
    QString name;

    // mouse tracking
    bool mouseTracking;

    /**
     * @brief Pomocna funkce pro navrace aktualne vybrane vrstvy s konrolou typu vrstvy
     * @param type - ID typu vrstvy. Pokud aktualne vybrana vrstvy neodpovida typu pak
     *  funkce navrati NULL.
     *  @param maskAllowed - Jsou povolene i masky jako navratove vrstvy
     * @return Layer
     */
    Layer *layerCheck(int type, bool maskAllowed = true);
private:
    Q_PROPERTY(Project *project READ getProject WRITE setProject NOTIFY projectChanged)
};

#endif // TOOL_H
