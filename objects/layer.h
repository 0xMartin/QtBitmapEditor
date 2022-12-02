#ifndef LAYER_H
#define LAYER_H

#include <QPaintEvent>

/**
 * @brief Abstraktni trida vrstvy. Vysledny bitmapovy obrazek se bude skladat z techto vrstev.
 */
class Layer
{
public:
    /**
     * @brief Konstruktor abstraktni tridy pro vrstvu
     * @param name - Jmeno vrstvy
     */
    Layer(const QString &name);

    virtual ~Layer();

    /**
     * @brief Nastavi viditelnost vrstvy
     * @param visibility - Viditelnost vrstvy
     */
    void setVisible(bool visibility);

    /**
     * @brief Navrati stav o tom zda je vrstva viditelna nebo ne
     * @return True-> vrstva je viditelna
     */
    bool isVisible() const;

    /**
     * @brief Nastavi naze vrstvy
     * @param name - Novy nazev vrstvy
     */
    void setName(const QString &name);

    /**
     * @brief Navrati nazev vrstvy
     * @return Nazev vrstvy
     */
    QString &getName();

    /**
     * @brief Paint event. Vykresli vrstvu do workspacu
     * @param painter - QPainter
     * @param offset - Offset vykreslovani
     */
    virtual void paintEvent(QPainter &painter, const QPoint &offset) = 0;

protected:
    bool visibility; /** Viditelnost vrstvy */
    QString name; /** Nazev vrstvy */

};

#endif // LAYER_H
