#ifndef LAYER_H
#define LAYER_H

#include <QObject>
#include <QPainter>
#include <QDebug>
#include <QBitmap>
#include <QDataStream>


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
 * @brief Abstraktni trida vrstvy. Reprezentuje jednu vrstvu obrazku ("projektu").
 */
class Layer : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Konstruktor abstraktni tridy pro vrstvu
     * @param project - Projekt ve kterem se vrstva nachazi
     * @param name - Jmeno vrstvy
     */
    Layer(QObject *project, const QString &name);

    virtual ~Layer();

    /**
     * @brief Navrati unikatni ID ktere jednoznacne urcuje typ vrstvy
     * @return ID
     */
    virtual qint32 getType() const;

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
    const QString &getName() const;

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
     * @brief Aplikuje masku vrstvy na painter
     * @param painter - QPainter
     */
    void applyLayerMask(QPainter &painter);

    /**
     * @brief Vytvori novou masku (aktualni masku automaticky odstrani z pameti)
     */
    void createMask();

    /**
     * @brief Zmeni velikost aktualni masky (zachova data)
     */
    void updateMaskSize();

    /**
     * @brief Odebere masku
     */
    void deleteMask();

    /**
     * @brief Vlozi jinou masku do aktualni masky
     * @param newMask - Jina maska
     */
    void maskPaste(QBitmap *newMask);

    /**
     * @brief Navrati masku vrtvy
     * @return QBitmap
     */
    QBitmap *getMask() const;

    /**
     * @brief Navrati stav o tom zda je maska vrstvy aktivni
     * @return True -> Je aktivni (bude se aplikovat pri vykreslovani)
     */
    bool isMaskActive() const;

    /**
     * @brief Nastavi stav aktivnosti masky vrstvy
     * @param newMaskActive - Stav
     */
    void setMaskActive(bool newMaskActive);

    /**
     * @brief Paint event. Vykresli vrstvu do workspacu
     * @param painter - QPainter
     */
    virtual void paintEvent(QPainter &painter) = 0;

    /**
     * @brief Vyrvori duplikat aktualni masky
     */
    QBitmap *duplicateMask() const;

    /**
     * @brief Vytvori duplikat vrstvy
     * @return Layer
     */
    virtual Layer *createDuplicate() const = 0;

    /**
     * @brief Serializuje vrstvu
     * @param stream - QDataStream (output stream)
     */
    virtual void serialize(QDataStream &stream);

    /**
     * @brief Deserializuje vrstvu. Pred samotnou deserializaci vrstvy je nutne
     * z input stream precit typ vrstvy (int)!!!
     * @param stream - QDataStream (input stream)
     */
    virtual void deserialize(QDataStream &stream);

protected:
    QString name; /** Nazev vrstvy */
    LayerBlendMode blendMode; /** Blend moder vrstvy */
    bool visibility; /** Viditelnost vrstvy */
    float opacity; /** Nepruhlednost vrstvy */
    bool antialiasing; /** Antialiasing povolen/zakazan */
    QBitmap *mask; /** Maska vrstvy */
    bool maskActive; /** True -> maska je aktivni pri vykreslovani */

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
                                     const QSize &size, const float step);


#endif // LAYER_H
