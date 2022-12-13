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
 * @brief The ColorPoint class
 */
class ColorPoint {
public:
    QColor color;
    float position;
    bool selected;

    ColorPoint(QColor color, float position);
};


/**
 * @brief The GradientEditor class
 */
class GradientEditor : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief GradientEditor
     * @param parent
     */
    explicit GradientEditor(QWidget *parent = NULL);
    ~GradientEditor();

    /**
     * @brief getGradient
     * @return
     */
    const QList<ColorPoint*> &getGradient() const;

    /**
     * @brief getAsLinearGradient
     * @param xStart
     * @param yStart
     * @param xFinalStop
     * @param yFinalStop
     * @return
     */
    QLinearGradient getAsLinearGradient(qreal xStart, qreal yStart, qreal xFinalStop, qreal yFinalStop) const;

    /**
     * @brief getAsRadialGradient
     * @param cx
     * @param cy
     * @param radius
     * @return
     */
    QRadialGradient getAsRadialGradient(qreal cx, qreal cy, qreal radius) const;

    /**
     * @brief getAsConicalGradient
     * @param cx
     * @param cy
     * @param startAngle
     * @return
     */
    QConicalGradient getAsConicalGradient(qreal cx, qreal cy, qreal startAngle) const;

public slots:
    /**
     * @brief clearGradient
     */
    void clearGradient();

    /**
     * @brief addColor
     */
    void addColor();

    /**
     * @brief changeColorOfSelected
     */
    void changeColorOfSelected();

    /**
     * @brief removeSelected
     */
    void removeSelected();

private slots:
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
