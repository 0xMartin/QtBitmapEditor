#include "mouseeventhelper.h"

#include <QtMath>

MouseEventHelper::MouseEventHelper(float moveUpdateDist)
{
    this->moveUpdateDist = moveUpdateDist * moveUpdateDist;
    this->moveUpdateDist = qMax(1.0, this->moveUpdateDist);
    this->lastPosSet = false;
    this->press = QPointF(0, 0);
    this->release = QPointF(0, 0);
    this->doubleClick = QPointF(0, 0);
    this->current = QPointF(0, 0);
    this->last = QPointF(0, 0);
    this->lastLDM = NEW;
}

void MouseEventHelper::updateDistance(float moveUpdateDist) {
    this->moveUpdateDist = moveUpdateDist * moveUpdateDist;
    this->moveUpdateDist = qMax(1.0, this->moveUpdateDist);
}

QPointF MouseEventHelper::getCurrent() const
{
    return this->current;
}

const QPointF *MouseEventHelper::getLast() const
{
    if(!this->lastPosSet && this->lastLDM == NEW) {
        return NULL;
    } else {
        return &this->last;
    }
}

QPointF MouseEventHelper::getPress() const
{
    return this->press;
}

QPointF MouseEventHelper::getRelease() const
{
    return this->release;
}

QPointF MouseEventHelper::getDoubleClick() const
{
    return this->doubleClick;
}

double MouseEventHelper::distFromPress()
{
    return qSqrt(qPow(this->current.x() - this->press.x(), 2) + qPow(this->current.y() - this->press.y(), 2));
}

double MouseEventHelper::distFromRelease()
{
    return qSqrt(qPow(this->current.x() - this->release.x(), 2) + qPow(this->current.y() - this->release.y(), 2));
}

double MouseEventHelper::distFromDoubleClick()
{
    return qSqrt(qPow(this->current.x() - this->doubleClick.x(), 2) + qPow(this->current.y() - this->doubleClick.y(), 2));
}

QLineF MouseEventHelper::lineFromPress()
{
    return QLineF(this->current, this->press);
}

QLineF MouseEventHelper::lineFromRelease()
{
    return QLineF(this->current, this->release);
}

QLineF MouseEventHelper::lineFromDoubleClick()
{
    return QLineF(this->current, this->doubleClick);
}

QLineF MouseEventHelper::lineFromLastPos()
{
    return QLineF(this->current, this->last);
}

QPointF MouseEventHelper::diffFromPress()
{
    return QPointF(this->current.x() - this->press.x(), this->current.y() - this->press.y());
}

QPointF MouseEventHelper::diffFromRelease()
{
    return QPointF(this->current.x() - this->release.x(), this->current.y() - this->release.y());
}

QPointF MouseEventHelper::diffFromDoubleClick()
{
    return QPointF(this->current.x() - this->doubleClick.x(), this->current.y() - this->doubleClick.y());
}

QPointF MouseEventHelper::diffFromLastPos()
{
    return QPointF(this->current.x() - this->last.x(), this->current.y() - this->last.y());
}

void MouseEventHelper::resetMove()
{
    this->lastPosSet = false;
    this->lastLDM = NEW;
}

MouseEventHelper MouseEventHelper::lastAction()
{
    return this->lastEvent;
}

void MouseEventHelper::processPressEvent(const QPointF &pos)
{
    this->press = pos;
    this->lastEvent = PRESS;
}

void MouseEventHelper::processReleaseEvent(const QPointF &pos)
{
    this->release = pos;
    this->lastEvent = RELEASE;
}

void MouseEventHelper::processDoubleClickEvent(const QPointF &pos)
{
    this->doubleClick = pos;
    this->lastEvent = DOUBLE_CLICK;
}

bool MouseEventHelper::processMoveEvent(const QPointF &pos)
{
    // aktualni pozice
    this->current = pos;

    // vypocet zmeny
    if(!this->lastPosSet) {
        switch (this->lastLDM) {
        case NEW:
            // nastaveni last pozice (nova)
            this->last = this->current;
            break;
        case LOAD:
            // nastaveni last pozice (nacte z ulozene hodnoty)
            this->last = this->last_tmp;
            break;
        }
        this->lastPosSet = true;
    } else {
        // pokud vzdalenost mezi last a current prekroci definovanou vzdalenost -> vrati TRUE
        // a nasledne v druhem volani teto tridy se last pozice znovu refreshne
        // moveUpdateDist -> je to druha mocnina
        if((qPow(this->current.x() - this->last.x(), 2) +
            qPow(this->current.y() - this->last.y(), 2)) >=
                this->moveUpdateDist) {
            // nyni uz hned nastavi "pocatecni pozici" (zamezeni trhane trajektorie pohybu)
            this->last_tmp = this->current;
            this->lastLDM = LOAD;
            this->lastPosSet = false;
            return true;
        }
    }

    return false;
}

float mapFunc(float value, float v_from, float v_to, float r_from, float r_to) {
    value = qMax(v_from, qMin(value, v_to));
    return ((value - v_from) / (v_to - v_from)) * (r_to - r_from) + r_from;
}

