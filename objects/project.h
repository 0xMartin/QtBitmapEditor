#ifndef PROJECT_H
#define PROJECT_H

#include <QString>

#include "layer.h"


// minimalni delka jmena projektu
#define PROJECT_MIN_NAME_LENGTH 3

// koncovka projektoveho souboru
#define PROJECT_FILE_EXTENSION ".qbe"


/**
 * @brief Datovy typ pro vektor vrstev obrazu
 */
typedef std::vector<Layer> Layers_t;

/**
 * @brief Trida udrzujici veskere informace o projektu. Pokud projekt pouziva nejake
 * externi zdroje (napr: obrazky) pak i ty se museji nachazet ve stejnem adresari jak projekt.
 */
class Project
{
protected:
    // jmeno projektu
    QString name;

    // cesta k souboru s ulozenym projektem
    QString path;

    // velikost obrazku
    QSize size;

    // vektor vrstev
    Layers_t layers;
public:
    /**
     * @brief Vytvori projekt
     * @param name - Nazev projektu
     * @param path - Cesta k projektu
     * @param size - Velikost obrazku
     */
    Project(const QString &name, const QString &path, const QSize &size);

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
    const Layers_t &getLayers() const;

    /**
     * @brief Ulozi projekt na disk
     * @return True -> projekt uspesne ulozen
     */
    bool saveProject() const;

    /**
     * @brief Paint event. Vykresli  projekt do workspacu (projekt = obrazek slozeny z vice vrstev)
     * @param painter - QPainter
     * @param offset - Offset vykreslovani
     */
    void paintEvent(QPainter &painter, const QPoint &offset);

    /**
     * @brief Stejne jako export event jen s tim rozdilem ze se do renderu navykresluje pomocna grafika editoru
     * @param painter - QPainter
     */
    void exportEvent(QPainter &painter);
};

#endif // PROJECT_H
