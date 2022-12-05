#include "workspace.h"


#include <QApplication>
#include <QPainter>
#include <QtMath>
#include <QScrollBar>


// velikost useku meritka v pixelech
#define RULE_STEP_PX 100

// inverzni scale
#define INV_SCALE(scale) (1.0 / scale)


Workspace::Workspace(QWidget *parent): QWidget(parent)
{
    this->project = NULL;
    this->scale = 1.0f;
    this->mouseHelper = MouseEventHelper(5);
    this->globalOffset = QPoint(0, 0);
    this->currentPos = QPoint(0, 0);

    this->setMinimumSize(QSize(400, 400));
    this->setBackgroundRole(QPalette::Base);
    this->setAutoFillBackground(true);

    this->font.setFamily("Monospace");
    this->font.setPixelSize(11);
    this->font.setStyle(QFont::StyleNormal);

    this->parentScrollArea = (QScrollArea*) parent;
}

void Workspace::setProject(Project *project) {
    this->project = project;
    this->project->setParent(this);
    this->repaint();
}

Project *Workspace::getProject() const {
    return this->project;
}

void Workspace::setScale(float scale)
{
    if(scale <= 0.0) return;
    if(scale > 100) return;
    this->scale = scale;
    this->repaint();
}

void Workspace::addScale(float diff)
{
    float f = this->scale + diff;
    if(f <= 0.0) return;
    if(f > 100) return;
    this->scale = f;
    this->repaint();
}

float Workspace::getScale() const
{
    return this->scale;
}

const QFont &Workspace::getFont() const
{
    return this->font;
}

void Workspace::setFont(const QFont &newFont)
{
    this->font = newFont;
}

void Workspace::mousePressEvent(QMouseEvent *event)
{
    switch (event->buttons()) {
    case Qt::LeftButton:
        // press event -> projekt
        if(this->project) {
            QPoint pos = this->calculateEventOffsetPosition(event->pos());
            if(pos.x() >= 0) {
                this->project->mousePressEvent(pos);
                this->repaint();
            }
        }
        break;
    case Qt::MiddleButton:
        this->setCursor(Qt::ClosedHandCursor);
        break;
    }
}

void Workspace::mouseReleaseEvent(QMouseEvent *event)
{
    // release event  -> projekt
    if(event->buttons() != Qt::LeftButton) {
        if(this->project) {
            QPoint pos = this->calculateEventOffsetPosition(event->pos());
            if(pos.x() >= 0) {
                this->project->mouseReleaseEvent(pos);
                this->repaint();
            }
        }
    }

    // release event -> zmena offsetu workspace pomoci stredoveho tlacitka
    if(event->buttons() != Qt::MiddleButton) {
        this->mouseHelper.resetMove();
        // nastaveni zakladniho kurzoru
        this->setCursor(Qt::ArrowCursor);
    }

}

void Workspace::mouseDoubleClickEvent(QMouseEvent *event)
{
    // double click -> projekt
    switch (event->buttons()) {
    case Qt::LeftButton:
        if(this->project) {
            QPoint pos = this->calculateEventOffsetPosition(event->pos());
            if(pos.x() >= 0) {
                this->project->mouseDoubleClickEvent(pos);
                this->repaint();
            }
        }
        break;
    }
}

void Workspace::mouseMoveEvent(QMouseEvent *event)
{
    this->currentPos = event->pos();

    switch (event->buttons()) {
    case Qt::LeftButton:
        // press event -> projekt
        if(this->project) {
            QPoint pos = this->calculateEventOffsetPosition(event->pos());
            if(pos.x() >= 0) {
                this->project->mouseMoveEvent(pos);
                this->repaint();
            }
        }
        break;
    case Qt::MiddleButton:
        // move event -> zmena offsetu workspace pomoci stredoveho tlacitka
        if(this->mouseHelper.processMoveEvent(event->pos())) {
            QPoint diff = this->mouseHelper.diffFromLastPos();
            this->globalOffset += diff;
            this->repaint();
        }
        break;
    }
}

void Workspace::wheelEvent(QWheelEvent *event)
{
    if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier) == true) {
        if(event->angleDelta().y() > 0) {
            this->addScale(0.1);
        } else {
            this->addScale(-0.1);
        }
        qDebug() << this->scale;
    }
}

void Workspace::paintEvent(QPaintEvent *event) {
    QPainter painter(this);


    // update velikosti
    updateSizeOfWorkaspace();


    // nastaveni kvality vykreslovani
    painter.setRenderHint(QPainter::Antialiasing, true);

    // vykresleni pozadi
    painter.fillRect(this->rect(), QBrush(QColor(37, 37, 37), Qt::SolidPattern));


    if(this->project != NULL) {
        // vypocet offsetu na stred celkove plochy workspacu
        QSize s = this->project->getSize();
        QPoint offset(
                    (this->width() - s.width()) / 2  + this->globalOffset.x(),
                    (this->height() - s.height()) / 2 + this->globalOffset.y()
                    );


        //-------PROJECT-------------------------
        painter.save();
        // aplikace offsetu
        painter.translate(offset);
        // vykresleni pozadi obrazku (sachovnice)
        Layer_paintBgGrid(painter, s * this->scale, 16);
        // aplikace scale
        painter.scale(this->scale, this->scale);
        // vykresleni projektu
        this->project->paintEvent(painter);
        // vraceni zpet do puvodniho stavu
        painter.restore();
        //-------PROJECT-------------------------


        // view port offset
        QPoint voff = this->getViewPortOffset();

        // vykresleni ramecku meritek
        painter.fillRect(26, voff.y(), this->width(), 26, QBrush(QColor(50, 50, 50), Qt::SolidPattern));
        painter.fillRect(voff.x(), 26, 26, this->height(), QBrush(QColor(50, 50, 50), Qt::SolidPattern));
        painter.setFont(this->font);
        painter.setPen(QColor(150, 150, 150));

        // x osa meritko
        int parts = qRound((float)s.width() / RULE_STEP_PX);
        int px_step = s.width() / parts;
        int step = px_step * this->scale;
        int from_start_to_0 = qCeil(offset.x() / RULE_STEP_PX);

        for(int x = offset.x() - step * from_start_to_0,
            px = -px_step * from_start_to_0;
            x < this->width();

            x+= step, px += px_step) {
            painter.drawText(QPointF(x, 18 + voff.y()), QString::number(px));
            painter.drawLine(x - 5, 4 + voff.y(), x - 5, 22 + voff.y());
        }


        // y osa meritko
        parts = qRound((float)s.height() / RULE_STEP_PX);
        px_step = s.height() / parts;
        step = px_step * this->scale;
        from_start_to_0 = qCeil((float)offset.y() / RULE_STEP_PX);
        QFontMetrics fm(this->font);

        for(int y = offset.y() - step * from_start_to_0,
            px = -px_step * from_start_to_0;
            y < this->height();
            y+= step, px += px_step) {

            QString num = QString::number(px);
            int i = 0;
            for(QChar &c : num) {
                painter.drawText(QPointF(8 + voff.x(), y + (i + 0.9) * (fm.height() - 4)), c);
                ++i;
            }
            painter.drawLine(4 + voff.x(), y - 5, 22 + voff.x(), y - 5);
        }

        // stredovy ramecek meritek
        painter.fillRect(voff.x(), voff.y(), 26, 26, QBrush(QColor(45, 45, 45), Qt::SolidPattern));


        // pozicni informace
        painter.setPen(QColor(250, 0, 0));
        Layer *l = this->project->getSelectedLayer();
        QString buffer = "Name: ";
        if(l) buffer += l->getName();
        painter.drawText(QPointF(
                             this->width() * INV_SCALE(this->scale) - 240 + voff.x(),
                             this->height() * INV_SCALE(this->scale) - 10 + voff.y()),
                         buffer);
        QPoint pos = this->calculateEventOffsetPosition(this->currentPos);
        buffer = "X: " + QString::number(pos.x()) + " Y: " + QString::number(pos.y());
        painter.drawText(QPointF(
                             this->width() * INV_SCALE(this->scale) - 110 + voff.x(),
                             this->height() * INV_SCALE(this->scale) - 10 + voff.y()),
                         buffer);
    }

}

void Workspace::updateSizeOfWorkaspace() {
    QSize size = this->parentWidget()->size();
    qDebug() << size;

    // pokud je velikost obrazku projetku vetsi nez velikost rodice pak tyto parametry prenastavi
    if(this->project) {
        QSize size2 = project->getSize();
        size *= this->scale;
        size.setWidth(qMax(size2.width(), size.width()));
        size.setHeight(qMax(size2.height(), size.height()));
    }

    // pokud dojde ke zmene velikost tak je prenastavi samotnemu workspacu
    if(this->size() != size) {
        this->setFixedSize(size);
    }
}

QPoint Workspace::calculateEventOffsetPosition(const QPoint &pos) const
{
    // workspace center offset : (widget.size - project.size) / 2
    QSize s = this->project->getSize();
    QPoint offset(-(this->width() - s.width()) / 2,
                  -(this->height() - s.height()) / 2);

    // mouse event offset
    offset += pos;

    // global offset
    offset -= this->globalOffset;

    // vynasobit inverznim scale
    // nyni uz finalni pozice prepoctana na soradnice v projektu
    offset *= INV_SCALE(this->scale);

    // pokud je mimo kreslici plochu tak zneplatni hodnoty
    if(offset.x() < 0 || offset.y() < 0 || offset.x() > s.width() || offset.y() > s.height()) {
        offset.setX(-1);
        offset.setY(-1);
    }

    return offset;
}

QPoint Workspace::getViewPortOffset() const
{
    return QPoint(this->parentScrollArea->horizontalScrollBar()->value(),
                  this->parentScrollArea->verticalScrollBar()->value()
                  );
}
