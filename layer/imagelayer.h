#ifndef IMAGELAYER_H
#define IMAGELAYER_H

#include <QPixmap>

#include "../base/layer.h"


#define IMAGE_LAYER_TYPE 1002


class ImageLayer : public Layer
{
    Q_OBJECT
public:
    QImage image; /** Obraz vrstvy */

    /**
     * @brief Prazdny konstruktor
     * @param project - Projekt ve kterem se vrstva nachazi
     */
    ImageLayer(QObject *project);

    /**
     * @brief Vytvori vrstvu z obrazkem
     * @param project - Projekt ve kterem se vrstva nachazi
     * @param name - Jmeno vrstvy
     * @param URL - URL adresa obrazku
     * @param size - Rozmery obrazku (v %, 100% -> puvodni velikost obrazku)
     */
    ImageLayer(QObject *project, const QString &name, const QString &URL, const QSize &size);

    /**
     * @brief Navrati rozmery obrazku
     * @return Rozmer Rozmery obrazku (v %, 100% -> puvodni velikost obrazku)
     */
    const QSize &getSize() const;

    /**
     * @brief Zmeni velikost obrazku
     * @param newSize - Rozmer Rozmery obrazku (v %, 100% -> puvodni velikost obrazku)
     */
    void setSize(const QSize &newSize);

    virtual void paintEvent(QPainter &painter) override;

    virtual qint32 getType() const override;

    virtual Layer *createDuplicate() const override;

    virtual void serialize(QDataStream &stream) override;

    virtual void deserialize(QDataStream &stream) override;

    void reloadImage();

protected:
    QString URL; /** Cesta k obrazku */
    QPoint position; /** Pozice obrazku */
    QSize size; /** Velikost obrazku (v %) */
};

#endif // BITMAPLAYER_H
