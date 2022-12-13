#ifndef BITMAPLAYER_H
#define BITMAPLAYER_H

#include <QPixmap>

#include "../base/layer.h"


#define BITMAP_LAYER_TYPE 1000


class BitmapLayer : public Layer
{
public:
    QImage image; /** Obraz */

    /**
     * @brief Vytvori bitmapovou vrstvu
     * @param parent - QObject
     * @param name - Jmeno vrstvy
     * @param size - Rozmery bitmapy
     */
    BitmapLayer(QObject *parent, const QString &name, const QSize &size);

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

    virtual int getType() override;

    virtual Layer *createDuplicate() const override;

protected:
    QSize size; /** Velikost bitmapy */
};

#endif // BITMAPLAYER_H
