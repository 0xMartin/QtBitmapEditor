#ifndef LAYER_H
#define LAYER_H

#include <QObject>
#include <QPainter>
#include <QDebug>


/**
 * @brief Rezim prolinani vrstev
 */
enum LayerBlendMode {
    NORMAL = 0,
    ADDITION = 1,
    MULTIPLY = 2,
    SCREEN = 3,
    OVERLAY = 4,
    DARKEN = 5,
    LIGHTEN = 6,
    COLOR_DODGE = 7,
    COLOR_BURN = 8,
    HARD_LIGHT = 9,
    SOFT_LIGHT = 10,
    DIFFERENCE = 11,
    EXCLUSION = 12
};


/**
 * @brief Abstraktni trida vrstvy. Reprezentuje jednu vrstvu bitmapoveho obrazku ("projektu").
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
     * @brief Povoli nebo zakaze Antialiasing
     * @param enabled - Povolen/Nepovolen
     */
    void enableAntialiasing(bool enabled);

    /**
     * @brief Je antialiasing povolen ?
     * @return Povolen/Nepovolen
     */
    bool isAntialiasingEnabled() const;

    /**
     * @brief Navrati aktualne nastaveny blend mode vrstvy
     * @return LayerBlendMode
     */
    LayerBlendMode getBlendMode() const;

    /**
     * @brief Zmeni blend mode teto vrstvy
     * @param newBlendMode - Novy blend mode
     */
    void setBlendMode(LayerBlendMode newBlendMode);

    /**
     * @brief Paint event. Vykresli vrstvu do workspacu
     * @param painter - QPainter
     */
    virtual void paintEvent(QPainter &painter) = 0;

    /**
     * @brief Navrati unikatni ID ktere jednoznacne urcuje typ vrstvy
     * @return ID
     */
    virtual int getType() = 0;

    /**
     * @brief Vytvori duplikat vrstvy
     * @return Layer
     */
    virtual Layer *createDuplicate() const = 0;

protected:
    QString name; /** Nazev vrstvy */
    LayerBlendMode blendMode;
    bool visibility; /** Viditelnost vrstvy */
    float opacity; /** Nepruhlednost vrstvy */
    bool antialiasing; /** Antialiasing povolen/zakazan */

};

/**
 * @brief Vykresleni "sachovnice" do pozadi
 * @param painter - QPainter
 * @param offset - Offset vykreslovane sachovnice (realna pozice ve viewportu)
 * @param viewPort - Velikost view portu
 * @param size - Velikost vykreslovane sachovnice (realna ve view portu)
 * @param step - Velikost jednoho ctverecku pozadi (relana ve view portu)
 */
Q_DECL_EXPORT void Layer_paintBgGrid(QPainter &painter, const QPoint &offset, const QSize &viewPort,
                                     const QSize &size, const size_t step);

#endif // LAYER_H
