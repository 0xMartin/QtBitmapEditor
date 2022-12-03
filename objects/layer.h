#ifndef LAYER_H
#define LAYER_H

#include <QObject>
#include <QPainter>
#include <QPixmap>

/**
 * @brief Abstraktni trida vrstvy. Reprezentuje jednu vrstvu bitmapoveho obrazku.
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
     * @brief Vyzada prekresleni
     */
    void requestRepaint();

    /**
     * @brief Navrati rozmery vrstvy (prebere z parent objektu = Projekt)
     * @return Rozmery vrstvy
     */
    QSize getSize() const;

    /**
     * @brief Paint event. Vykresli vrstvu do workspacu
     * @param painter - QPainter
     */
    virtual void paintEvent(QPainter &painter) = 0;

protected:
    QString name; /** Nazev vrstvy */
    bool visibility; /** Viditelnost vrstvy */
    float opacity; /** Nepruhlednost vrstvy */
};

/**
 * @brief Vykresleni "sachovnice" do pozadi
 * @param painter - QPainter
 * @param size - Velikost pozadi vrstvy
 * @param step - Velikost jednoho ctverecku pozadi
 */
Q_DECL_EXPORT void Layer_paintBgGrid(QPainter &painter, const QSize &size, const size_t step);

#endif // LAYER_H
