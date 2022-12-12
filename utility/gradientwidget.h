#ifndef GRADIENTWIDGET_H
#define GRADIENTWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QColor>
#include <QRadialGradient>
#include <QLinearGradient>
#include <QConicalGradient>


/**
 * @brief The ColorPoint class
 */
class ColorPoint {
public:
    QColor color;
    float position;

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

    /**
     * @brief getGradient
     * @return
     */
    const QList<ColorPoint> &getGradient() const;

    /**
     * @brief clearGradient
     */
    void clearGradient();

public slots:
    /**
     * @brief changeColor
     * @param index
     */
    void changeColor(qsizetype index);

    /**
     * @brief changePosition
     * @param index
     * @param position
     */
    void changePosition(qsizetype index, float position);

protected:
    QList<ColorPoint> gradient; /** List s barvamy - gradient */

    // events
    void paintEvent(QPaintEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // GRADIENTWIDGET_H
