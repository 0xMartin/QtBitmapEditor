#ifndef TEXTLAYER_H
#define TEXTLAYER_H

#include <QPixmap>

#include "../base/layer.h"


#define TEXT_LAYER_TYPE 1001


class TextLayer : public Layer
{
public:
    QString text; /** Zobrazovany text */
    QFont font; /** Font textu */
    QColor color; /** Barva textu */
    QPoint position; /** Pozice textu*/

    /**
     * @brief Vytvori bitmapovou vrstvu
     * @param parent - QObject
     * @param name - Jmeno vrstvy
     * @param size - Rozmery bitmapy
     * @param text - Inicializacni text
     */
    TextLayer(QObject *parent, const QString &name, const QString &text);

    virtual void paintEvent(QPainter &painter) override;

    virtual int getType() override;

    virtual Layer *createDuplicate() const override;

};

#endif // BITMAPLAYER_H
