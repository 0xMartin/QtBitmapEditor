#ifndef PROJECT_H
#define PROJECT_H

#include <QString>

#include "layer.h"

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

    // velikost obrazku
    QSize size;

    // vektor vrstev
    Layers_t layers;
public:
    /**
     * @brief Project
     * @param name
     * @param size
     */
    Project(const QString &name, const QSize &size);

    /**
     * @brief getName
     * @return
     */
    const QString &getName() const;

    /**
     * @brief setName
     * @param name
     * @return
     */
    bool setName(const QString &name);

    /**
     * @brief setSize
     * @param size
     */
    bool setSize(const QSize &size);

    /**
     * @brief getSize
     * @return
     */
    QSize &getSize();


    /**
     * @brief Navrati referenci na vektr vrstev
     * @return Layers_t
     */
    const Layers_t &getLayers() const;

    /**
     * @brief Paint event. Vykresli  projekt do workspacu (projekt = obrazek slozeny z vice vrstev)
     * @param event - QPaintEvent
     */
    void paintEvent(QPaintEvent *event);

    /**
     * @brief Stejne jako export event jen s tim rozdilem ze se do renderu navykresluje pomocna grafika editoru
     * @param event - QPaintEvent
     */
    void exportEvent(QPaintEvent *event);
};

#endif // PROJECT_H
