#ifndef MOUSEEVENTHELPER_H
#define MOUSEEVENTHELPER_H

#include <QPointF>
#include <QMouseEvent>
#include <QLineF>

enum MouseEventHelperAction {
    NONE,
    PRESS,
    RELEASE,
    DOUBLE_CLICK
};

/**
 * @brief Helper trida pro mouse eventy
 */
class MouseEventHelper
{
public:

    /**
     * @brief Helper trida pro mouse eventy
     * @param moveUpdateDist - Minimalni vzdalenost od predchozi zaznamenane pozice pro move event
     */
    MouseEventHelper(float moveUpdateDist = 5);

    /**
     * @brief Prepocita move update distance
     * @param moveUpdateDist - Vzdalenost
     */
    void updateDistance(float moveUpdateDist);

    /**
     * @brief Pozice mouse press eventu
     * @return QPointF
     */
    QPointF getPress() const;

    /**
     * @brief Pozice mouse release eventu
     * @return QPointF
     */
    QPointF getRelease() const;

    /**
     * @brief Pozice mouse double click eventu
     * @return QPointF
     */
    QPointF getDoubleClick() const;

    /**
     * @brief Aktulani pozice kurzoru
     * @return QPointF
     */
    QPointF getCurrent() const;

    /**
     * @brief Posledni zaznamenana pozice v move eventu
     * @return QPointF
     */
    const QPointF *getLast() const;

    /**
     * @brief Vzdalenost od pozice kde naposledy byl zaznamenan press event
     * @return Vzdalenost
     */
    double distFromPress();

    /**
     * @brief Vzdalenost od pozice kde naposledy byl zaznamenan release event
     * @return Vzdalenost
     */
    double distFromRelease();

    /**
     * @brief Vzdalenost od pozice kde naposledy byl zaznamenan double click event
     * @return Vzdalenost
     */
    double distFromDoubleClick();

    /**
     * @brief Vytvori QLineF <current, press>
     * @return QLineF
     */
    QLineF lineFromPress();

    /**
     * @brief Vytvori QLineF <current, release>
     * @return QLineF
     */
    QLineF lineFromRelease();

    /**
     * @brief Vytvori QLineF <current, double click>
     * @return QLineF
     */
    QLineF lineFromDoubleClick();

    /**
     * @brief Vytvori QLineF <current, last>
     * @return QLineF
     */
    QLineF lineFromLastPos();

    /**
     * @brief Vypocita diferenci [press - last]
     * @return QPointF
     */
    QPointF diffFromPress();

    /**
     * @brief Vypocita diferenci [release - last]
     * @return QPointF
     */
    QPointF diffFromRelease();

    /**
     * @brief Vypocita diferenci [double_click - last]
     * @return QPointF
     */
    QPointF diffFromDoubleClick();

    /**
     * @brief Vypocita diferenci [current - last]
     * @return QPointF
     */
    QPointF diffFromLastPos();

    /**
     * @brief Resetuje move event (reset merene vzdalenosti)
     */
    void resetMove();

    /**
     * @brief Navrati posledni zpracovanou akci (ignoruje detekci eventu MOVE)
     * @return MouseEventHelper
     */
    MouseEventHelper lastAction();

    // procedury pro zpracovani jednotlivych eventu (vzdy volat)
    void processPressEvent(const QPointF &pos);
    void processReleaseEvent(const QPointF &pos);
    void processDoubleClickEvent(const QPointF &pos);
    bool processMoveEvent(const QPointF &pos); /** Navrati TRUE pokud byla pozice aktualizovana*/

protected:
    float moveUpdateDist; /** Minimalni vzdalenost od predchozi zaznamenane pozice pro move event */
    bool lastPosSet;

    MouseEventHelperAction lastEvent = NONE; /** Posledni zaznamenany event */

    QPointF press; /** Pozice mouse press eventu */
    QPointF release; /** Pozice mouse release eventu */
    QPointF doubleClick; /** Pozice mouse double click eventu */
    QPointF current; /** Aktulani pozice kurzoru */
    QPointF last; /** Predchozi pozice kurzoru */

private:
    enum LastLoadMode {
        NEW, LOAD
    };

    LastLoadMode lastLDM; /** Zpusop nacteni last pozice */
    QPointF last_tmp; /** Docasna promenna pro last */
};

/**
 * @brief Map funkce. Prevede hodnotu z jednoho rozsahu hodnot do druheho
 * @param value - Hodnota ktera bude prevadena
 * @param v_from - Pocatecni hodnota vstupniho rozsahu
 * @param v_to - Koncova hodnota vstupniho rozsahu
 * @param r_from - Pocatecni hodnota vystupniho rozsahu
 * @param r_to - Koncova hodnota vystupniho rozsahu
 * @return
 */
Q_DECL_EXPORT float mapFunc(float value, float v_from, float v_to, float r_from, float r_to);

#endif // MOUSEEVENTHELPER_H
