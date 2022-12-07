#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include <QString>
#include <QList>

#include "layer.h"


// minimalni delka jmena projektu
#define PROJECT_MIN_NAME_LENGTH 3

// koncovka projektoveho souboru
#define PROJECT_FILE_EXTENSION ".qbe"


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
    QSize &getSize();

    /**
     * @brief Navrati referenci na vektr vrstev
     * @return Layers_t
     */
    Layers_t *getLayers() const;

    /**
     * @brief Prida novou vrstvu do projektu. Vrstavy budou odstraneni
     * z pameti pri volani destruktoru projektu.
     * @param layer - Pointer na vrstvu
     * @return True -> vrstva uspesne pridana
     */
    bool addLayer(Layer *layer);

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
     * @brief Paint event. Vykresli  projekt do workspacu (projekt = obrazek slozeny z vice vrstev)
     * @param offset - Offset vykreslovani
     */
    void paintEvent(QPainter &painter);

signals:
    /**
     * @brief Signal je vyvolan ve chvili kdy doslo k prekresleni nejak z vrstev projektu
     * @param layer - Vrsrva ktera byla prekreslena
     */
    void repaintSignal(Layer *layer);

protected:
    // jmeno projektu
    QString name;

    // cesta k souboru s ulozenym projektem
    QString path;

    // velikost obrazku
    QSize size;

    // vektor vrstev
    Layers_t *layers;

    // aktualne vybrana vrstva (focus)
    Layer *selected_layer;

};

#endif // PROJECT_H
