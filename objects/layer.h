#ifndef LAYER_H
#define LAYER_H

#include <QObject>
#include <QPaintEvent>

/**
 * @brief Abstraktni trida vrstvy. Vysledny bitmapovy obrazek se bude skladat z techto vrstev.
 */
class Layer : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Konstruktor abstraktni tridy pro vrstvu
     * @param parent - QObject
     * @param name - Jmeno vrstvy
     */
    Layer(QObject *parent, const QString &name);

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
     * @brief Navrati nepruhlednost vrsty
     * @return Nepruhlednost (opacity)
     */
    float getOpacity() const;

    /**
     * @brief Nastavi nepruhlednost vrsty
     * @param newOpacity - Nepruhlednost
     */
    void setOpacity(float newOpacity);

    /**
     * @brief Paint event. Vykresli vrstvu do workspacu
     * @param painter - QPainter
     * @param offset - Offset vykreslovani
     */
    virtual void paintEvent(QPainter &painter, const QPoint &offset) = 0;

protected:
    QString name; /** Nazev vrstvy */
    bool visibility; /** Viditelnost vrstvy */
    float opacity; /** Nepruhlednost vrstvy */
};

#endif // LAYER_H
