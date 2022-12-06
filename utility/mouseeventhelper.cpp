#include "mouseeventhelper.h"

#include <QtMath>

MouseEventHelper::MouseEventHelper(int moveUpdateDist)
{
    this->moveUpdateDist = moveUpdateDist * moveUpdateDist;
    this->lastPosSet = false;
    this->press = QPoint(0, 0);
    this->release = QPoint(0, 0);
    this->doubleClick = QPoint(0, 0);
    this->current = QPoint(0, 0);
    this->last = QPoint(0, 0);
    this->lastLDM = NEW;
}

QPoint MouseEventHelper::getCurrent() const
{
    return this->current;
}

const QPoint *MouseEventHelper::getLast() const
{
    if(!this->lastPosSet && this->lastLDM == NEW) {
        return NULL;
    } else {
        return &this->last;
    }
}

QPoint MouseEventHelper::getPress() const
{
    return this->press;
}

QPoint MouseEventHelper::getRelease() const
{
    return this->release;
}

QPoint MouseEventHelper::getDoubleClick() const
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

QLine MouseEventHelper::lineFromPress()
{
    return QLine(this->current, this->press);
}

QLine MouseEventHelper::lineFromRelease()
{
    return QLine(this->current, this->release);
}

QLine MouseEventHelper::lineFromDoubleClick()
{
    return QLine(this->current, this->doubleClick);
}

QLine MouseEventHelper::lineFromLastPos()
{
    return QLine(this->current, this->last);
}

QPoint MouseEventHelper::diffFromPress()
{
    return QPoint(this->current.x() - this->press.x(), this->current.y() - this->press.y());
}

QPoint MouseEventHelper::diffFromRelease()
{
    return QPoint(this->current.x() - this->release.x(), this->current.y() - this->release.y());
}

QPoint MouseEventHelper::diffFromDoubleClick()
{
    return QPoint(this->current.x() - this->doubleClick.x(), this->current.y() - this->doubleClick.y());
}

QPoint MouseEventHelper::diffFromLastPos()
{
    return QPoint(this->current.x() - this->last.x(), this->current.y() - this->last.y());
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

void MouseEventHelper::processPressEvent(const QPoint &pos)
{
    this->press = pos;
    this->lastEvent = PRESS;
}

void MouseEventHelper::processReleaseEvent(const QPoint &pos)
{
    this->release = pos;
    this->lastEvent = RELEASE;
}

void MouseEventHelper::processDoubleClickEvent(const QPoint &pos)
{
    this->doubleClick = pos;
    this->lastEvent = DOUBLE_CLICK;
}

bool MouseEventHelper::processMoveEvent(const QPoint &pos)
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

