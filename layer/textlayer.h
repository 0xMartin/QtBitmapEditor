#ifndef TEXTLAYER_H
#define TEXTLAYER_H

#include <QPixmap>

#include "../base/layer.h"


#define TEXT_LAYER_TYPE 1001


/**
 * @brief Textova vrstva. Obsahuje jen poze jeden retezec.
 */
class TextLayer : public Layer
{
    Q_OBJECT
public:

    /**
     * @brief Prazdny konstruktor
     * @param project - Projekt ve kterem se vrstva nachazi
     */
    TextLayer(QObject *project);

    /**
     * @brief Vytvori bitmapovou vrstvu
     * @param project - Projekt ve kterem se vrstva nachazi
     * @param name - Jmeno vrstvy
     * @param size - Rozmery bitmapy
     * @param text - Inicializacni text
     * @param pos - Pozice textu
     */
    TextLayer(QObject *project, const QString &name,
              const QString &text, const QPoint &pos);

    virtual void paintEvent(QPainter &painter) override;

    virtual qint32 getType() const override;

    virtual Layer *createDuplicate() const override;

    virtual void serialize(QDataStream &stream) override;

    virtual void deserialize(QDataStream &stream) override;


    /**GET/SET**********************************************/
    const QString &getText() const;
    void setText(const QString &newText);

    const QFont &getFont() const;
    void setFont(const QFont &newFont);

    const QColor &getColor() const;
    void setColor(const QColor &newColor);

    QPoint getPosition() const;
    void setPosition(QPoint newPosition);
    /**GET/SET**********************************************/

protected:
    QString text; /** Zobrazovany text */
    QFont font; /** Font textu */
    QColor color; /** Barva textu */
    QPoint position; /** Pozice textu*/

};

#endif // BITMAPLAYER_H
