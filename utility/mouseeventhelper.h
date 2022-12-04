#ifndef MOUSEEVENTHELPER_H
#define MOUSEEVENTHELPER_H

#include <QPoint>
#include <QMouseEvent>
#include <QLine>

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
    MouseEventHelper(int moveUpdateDist = 5);

    /**
     * @brief Pozice mouse press eventu
     * @return QPoint
     */
    QPoint getPress() const;

    /**
     * @brief Pozice mouse release eventu
     * @return QPoint
     */
    QPoint getRelease() const;

    /**
     * @brief Pozice mouse double click eventu
     * @return QPoint
     */
    QPoint getDoubleClick() const;

    /**
     * @brief Aktulani pozice kurzoru
     * @return QPoint
     */
    QPoint getCurrent() const;

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
     * @brief Vytvori QLine <current, press>
     * @return QLine
     */
    QLine lineFromPress();

    /**
     * @brief Vytvori QLine <current, release>
     * @return QLine
     */
    QLine lineFromRelease();

    /**
     * @brief Vytvori QLine <current, double click>
     * @return QLine
     */
    QLine lineFromDoubleClick();

    /**
     * @brief Vytvori QLine <current, last>
     * @return QLine
     */
    QLine lineFromLastPos();

    /**
     * @brief Vypocita diferenci [press - last]
     * @return QPoint
     */
    QPoint diffFromPress();

    /**
     * @brief Vypocita diferenci [release - last]
     * @return QPoint
     */
    QPoint diffFromRelease();

    /**
     * @brief Vypocita diferenci [double_click - last]
     * @return QPoint
     */
    QPoint diffFromDoubleClick();

    /**
     * @brief Vypocita diferenci [current - last]
     * @return QPoint
     */
    QPoint diffFromLastPos();

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
    void processPressEvent(const QPoint &pos);
    void processReleaseEvent(const QPoint &pos);
    void processDoubleClickEvent(const QPoint &pos);
    bool processMoveEvent(const QPoint &pos); /** Navrati TRUE pokud byla pozice aktualizovana*/

protected:
    int moveUpdateDist; /** Minimalni vzdalenost od predchozi zaznamenane pozice pro move event */
    bool lastPosSet;

    MouseEventHelperAction lastEvent = NONE; /** Posledni zaznamenany event */

    QPoint press; /** Pozice mouse press eventu */
    QPoint release; /** Pozice mouse release eventu */
    QPoint doubleClick; /** Pozice mouse double click eventu */
    QPoint current; /** Aktulani pozice kurzoru */
    QPoint last; /** Predchozi pozice kurzoru */
};

#endif // MOUSEEVENTHELPER_H
