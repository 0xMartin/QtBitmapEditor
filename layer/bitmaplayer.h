#ifndef BITMAPLAYER_H
#define BITMAPLAYER_H

#include <QPixmap>

#include "../base/layer.h"


#define BITMAP_LAYER_TYPE 1000


class BitmapLayer : public Layer
{
    Q_OBJECT
public:
    QImage image; /** Obraz vrstvy */

    /**
     * @brief Prazdny konstruktor
     * @param project - Projekt ve kterem se vrstva nachazi
     */
    BitmapLayer(QObject *project);

    /**
     * @brief Vytvori bitmapovou vrstvu z obrazku
     * @param project - Projekt ve kterem se vrstva nachazi
     * @param name - Jmeno vrstvy
     * @param URL - URL obrazku ze ktereho vrstvu vytvori
     */
    BitmapLayer(QObject *project, const QString &name, const QString &URL);

    /**
     * @brief Vytvori bitmapovou vrstvu
     * @param project - Projekt ve kterem se vrstva nachazi
     * @param name - Jmeno vrstvy
     * @param size - Rozmery bitmapy
     */
    BitmapLayer(QObject *project, const QString &name, const QSize &size);

    /**
     * @brief Navrati rozmery bitmapy
     * @return Rozmer bitmapy
     */
    const QSize &getSize() const;

    /**
     * @brief Zmeni velikost bitmapy
     * @param newSize - Nova velikost bitmapy
     */
    void setSize(const QSize &newSize);

    virtual void paintEvent(QPainter &painter) override;

    virtual qint32 getType() const override;

    virtual Layer *createDuplicate() const override;

    virtual void serialize(QDataStream &stream) override;

    virtual void deserialize(QDataStream &stream) override;

protected:
    QSize size; /** Velikost bitmapy */
};

/**
 * @brief Rasterizuje vybranou vrstvu vrstvu
 * @param layer - Vrstva
 */
Q_DECL_EXPORT BitmapLayer  *ResterizeLayer(Layer *layer);

#endif // BITMAPLAYER_H
