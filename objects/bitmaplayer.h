#ifndef BITMAPLAYER_H
#define BITMAPLAYER_H

#include <QPixmap>

#include "layer.h"

class BitmapLayer : public Layer
{
public:
    /**
     * @brief Vytvori bitmapovou vrstvu
     * @param name - Jmeno vrstvy
     * @param size - Rozmery bitmapy
     */
    BitmapLayer(const QString &name, const QSize &size);

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

    /**
     * @brief Navrati ukazatel na pixel mapu
     * @return QPixelmap
     */
    const QPixmap &getPixmap() const;

    virtual void paintEvent(QPainter &painter, const QPoint &offset) override;

protected:
    QSize size; /** Velikost bitmapy */
    QPixmap pixmap; /** Bitmapa */
};

#endif // BITMAPLAYER_H
