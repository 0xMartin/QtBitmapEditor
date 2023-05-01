#ifndef GRADIENTWIDGET_H
#define GRADIENTWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QColor>
#include <QRadialGradient>
#include <QLinearGradient>
#include <QConicalGradient>

#include "mouseeventhelper.h"


/**
 * @brief Barva s pozici v gradientu
 */
class ColorPoint {
public:
    QColor color;
    float position;
    bool selected;

    ColorPoint(QColor color, float position);
};


/**
 * @brief Nastroj pro editovani gradientu
 */
class GradientEditor : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief Nastroj pro editovani gradientu
     * @param parent - Parent QWidget
     */
    explicit GradientEditor(QWidget *parent = NULL);
    ~GradientEditor();

    /**
     * @brief Navrati list obsahujici barvy a jijich pozice gradientu
     * @return QList<ColorPoint*>
     */
    const QList<ColorPoint*> &getGradient() const;

    /**
     * @brief Z konfigurace vytvori linearni gradient
     * @param xStart - Start pozice (x) gradientu
     * @param yStart - Start pozice (y) gradientu
     * @param xFinalStop - Koncova pozice (x) gradientu
     * @param yFinalStop - Koncova pozice (y) gradientu
     * @return QLinearGradient
     */
    QLinearGradient getAsLinearGradient(qreal xStart, qreal yStart, qreal xFinalStop, qreal yFinalStop) const;

    /**
     * @brief Z konfigurace vytvori radialni gradient
     * @param cx - Center pozice (x)
     * @param cy - Center pozice (y)
     * @param radius - Radius kruhu
     * @return QRadialGradient
     */
    QRadialGradient getAsRadialGradient(qreal cx, qreal cy, qreal radius) const;

    /**
     * @brief Z konfigurace vytvori konicky gradient
     * @param cx - Center pozice (x)
     * @param cy - Center pozice (y)
     * @param startAngle - Pocatecni uhel
     * @return QConicalGradient
     */
    QConicalGradient getAsConicalGradient(qreal cx, qreal cy, qreal startAngle) const;

public slots:
    /**
     * @brief Odstrani vsechny barvy z gradientu
     */
    void clearGradient();

    /**
     * @brief Prida novou barvu do gradientu
     */
    void addColor();

    /**
     * @brief Zmeni barvu aktualne vybraneho bodu v gradientu
     */
    void changeColorOfSelected();

    /**
     * @brief Odstrani aktualne vybrany bod (barvu)
     */
    void removeSelected();

private slots:
    /**
     * @brief Zobrazi kontextove menu (2 varianty - 1. pro vybranou barvu - 2. pro gradient jako celek)
     * @param pos - Pozice kurzoru
     */
    void showContextMenu(const QPoint &pos);

protected:
    QList<ColorPoint*> gradient; /** List s barvamy - gradient */

    MouseEventHelper mouseHelper;

    // events
    void paintEvent(QPaintEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // GRADIENTWIDGET_H
