#ifndef BITMAPLAYER_H
#define BITMAPLAYER_H

#include <QPixmap>

#include "../base/layer.h"
#include "../utility/mouseeventhelper.h"

class BitmapLayer : public Layer
{
public:
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

    /**
     * @brief Navrati ukazatel na pixel mapu
     * @return QPixelmap
     */
    const QPixmap &getPixmap() const;

    virtual void paintEvent(QPainter &painter) override;

    // events
    virtual void mousePressEvent(const QPoint &pos) override;
    virtual void mouseReleaseEvent(const QPoint &pos) override;
    virtual void mouseDoubleClickEvent(const QPoint &pos) override;
    virtual void mouseMoveEvent(const QPoint &pos) override;
    virtual void outOfAreaEvent(const QPoint &pos) override;

protected:
    QSize size; /** Velikost bitmapy */
    QPixmap pixmap; /** Bitmapa */

    // helper pro mouse eventy
    MouseEventHelper mouseHelper;
};

#endif // BITMAPLAYER_H
