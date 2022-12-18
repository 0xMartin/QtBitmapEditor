#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QString>
#include <QList>

#include "layer.h"


/**
 * @brief Pracovni mode projektu
 */
enum ProjectEditMode_t {
    PROJECT_EDIT, /** Zakladni mod, editovani projektu. Vsechny vrstvy se vykresluji tak jak realne budou vypadat na obrazku */
    MASK_EDIT /** Mod pro editaci masek. Je videt vzdy jen maska aktualne vybrane vrstvy */
};


/**
 * @brief Datovy typ pro vektor vrstev obrazu
 */
typedef QList<Layer*> Layers_t;

/**
 * @brief Trida udrzujici veskere informace o projektu. Pokud projekt pouziva nejake
 * externi zdroje (napr: obrazky) pak i ty se museji nachazet ve stejnem adresari jak projekt.
 */
class Project : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Vytvori projekt
     * @param parent - QObject
     * @param name - Nazev projektu
     * @param path - Cesta k projektu
     * @param size - Velikost obrazku
     */
    Project(QObject *parent, const QString &name, const QString &path, const QSize &size);

    ~Project();

    /**
     * @brief Navrati nazev projektu
     * @return Nazev projektu
     */
    const QString &getName() const;

    /**
     * @brief Nastavi nove jmeno projektu
     * @param name - Nove jmemo projektu (minimalni delka 3 znaky)
     * @return True -> jmeno bylo uspesne nastaveno
     */
    bool setName(const QString &name);

    /**
     * @brief Navrati cestu k souboru s projektem ulozenem na disku
     * @return Cesta k souboru
     */
    const QString &getPath() const;

    /**
     * @brief Navrati absolutni cestu k adresari ve kterem se projekt nachazi
     * @return Absolutni cesta
     */
    QString getDirPath();

    /**
     * @brief Nastavi cestu k souboru projektu (misto ulozeni na disku)
     * @param path - Cesta k souboru
     */
    void setPath(const QString &path);

    /**
     * @brief Nastavi velikost obrazku
     * @param size - Velikost obrazku
     */
    bool setSize(const QSize &size);

    /**
     * @brief Navrati velikost obrazku
     * @return Velikost obrazku
     */
    const QSize &getSize() const;

    /**
     * @brief Navrati referenci na vektr vrstev
     * @return Layers_t
     */
    Layers_t *getLayers() const;

    /**
     * @brief Prida novou vrstvu do projektu (jako horni vrstvu). Vrstavy budou odstraneni
     * z pameti pri volani destruktoru projektu.
     * @param layer - Pointer na vrstvu
     * @return True -> vrstva uspesne pridana
     */
    bool addLayerAtTop(Layer *layer);

    /**
     * @brief Prida novou vrstvu do projektu na definovene misto. Vrstavy budou odstraneni
     * z pameti pri volani destruktoru projektu.
     * @param index - Index vrstvy
     * @param layer - Pointer na vrstvu
     * @return True -> vrstva uspesne pridana
     */
    bool insertLayer(qsizetype index, Layer *layer);

    /**
     * @brief Prida novou vrstvu do projektu nad aktualne oznacenou. Vrstavy budou odstraneni
     * z pameti pri volani destruktoru projektu.
     * @param layer - Pointer na vrstvu
     * @return True -> vrstva uspesne pridana
     */
    bool insertLayerAbove(Layer *layer);

    /**
     * @brief Odebere vrstvu z projektu. Automtaticky zavola i jeji destruktor
     * @param layer - Pointer na vrstvu
     * @return True -> vrstva uspesne odebrana
     */
    bool removeLayer(Layer *layer);

    /**
     * @brief Ulozi projekt na disk
     * @return True -> projekt uspesne ulozen
     */
    bool saveProject() const;

    /**
     * @brief Navrati aktualne vybranou vrstvu
     * @return Layer
     */
    Layer *getSelectedLayer() const;

    /**
     * @brief Nastavi aktualne vybranou vrstvu
     * @param newSelected_layer - Layer
     */
    void setSelectedLayer(Layer *newSelected_layer);

    /**
     * @brief Vyzada prekresleni
     */
    void requestRepaint();

    /**
     * @brief Presun aktualne vybrane vrstvy o jedno nahoru (blize k zacantku listu: index 0)
     * @return True -> Vrstva uspesne posunuta
     */
    bool moveSelectedLayerUp();

    /**
     * @brief Presun aktualne vybrane vrstvy o jedno dolu (blize ke konci listu: index end)
     * @return True -> Vrstva uspesne posunuta
     */
    bool moveSelectedLayerDown();

    /**
     * @brief Aktualne vybranou vrstvu spoji s tou, ktera se nachazi ve vykreslovani pod ni (nizsi index v listu)
     * @return True -> uspesne provedeno
     */
    bool mergaSeletedLayerDown();

    /**
     * @brief Vytvori duplikat aktualne vybrane vrstvy
     * @return True -> uspesne provedeno
     */
    bool dupliceteLayer();

    /**
     * @brief Rasterizuje aktulane vybranou vrstvu
     * @return True -> uspesne provedeno
     */
    bool rasterizeLayer();

    /**
     * @brief Navrati pracovni mod
     * @return WorkspaceMode_t
     */
    ProjectEditMode_t getMode() const;

    /**
     * @brief Nastavi novy pracovni mod
     * @param newMode - WorkspaceMode_t
     */
    void setMode(ProjectEditMode_t newMode);

    /**
     * @brief Paint event. Vykresli  projekt do workspacu (projekt = obrazek slozeny z vice vrstev)
     * @param offset - Offset vykreslovani
     */
    void paintEvent(QPainter &painter);

signals:
    /**
     * @brief Signal je vyvolan ve chvili kdy doslo k prekresleni nejake z vrstev projektu
     * (zmena jejiho obsahu!! ne vykresleni do workspacu) [volano externe z workspacu]
     * @param layer - Vrsrva ktera byla prekreslena
     */
    void repaintSignal(Layer *layer);

    /**
     * @brief Signal je vyvolan ve chvili kdy je pozmenen list vrstev projektu
     */
    void layerListChanged();

protected:
    // jmeno projektu
    QString name;

    // cesta k souboru s ulozenym projektem
    QString path;

    // velikost obrazu (vsechny vrstvy maji stejnou velikost jak projekt)
    QSize size;

    // list vrstev
    Layers_t *layers;

    // aktualne vybrana vrstva (focus)
    Layer *selected_layer;

    // pracovni mode
    ProjectEditMode_t mode;

};

/**
 * @brief Zapise projekt do souboru
 * @param project - Projekt
 */
Q_DECL_EXPORT void WriteProjectToFile(const Project *project);

/**
 * @brief Precte projekt ze souboru
 * @param projectPath - Cesta k projektu
 * @return Projekt
 */
Q_DECL_EXPORT Project *ReadProjectFromFile(const QString &projectPath);

#endif // PROJECT_H
