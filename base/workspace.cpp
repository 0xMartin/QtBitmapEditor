#include "workspace.h"


#include <QApplication>
#include <QPainter>
#include <QtMath>
#include <QScrollBar>


// velikost useku meritka v pixelech
#define RULE_STEP_PX_MIN 80

// inverzni scale
#define INV_SCALE(scale) (1.0 / scale)


Workspace::Workspace(const Config_Workspace_t &config, QWidget *parent): QWidget(parent)
{
    this->config = config;
    this->project = NULL;
    this->scale = 1.0f;
    this->mouseHelper = MouseEventHelper(5);
    this->globalOffset = QPoint(0, 0);
    this->currentPos = QPoint(0, 0);

    this->setMinimumSize(QSize(400, 400));
    this->setBackgroundRole(QPalette::Base);
    this->setAutoFillBackground(true);
}

void Workspace::setProject(Project *project) {
    this->project = project;
    this->project->setParent(this);

    // vypocet vhodneho meritka pri prvnim nacteni projektu
    if(this->project->getSize().width() > this->project->getSize().height()) {
        this->setScale(this->width() * 0.6 / this->project->getSize().width());
    } else {
        this->setScale(this->scale = this->height() * 0.6 / this->project->getSize().height());
    }
    this->repaint();
}

Project *Workspace::getProject() const {
    return this->project;
}

void Workspace::setScale(float scale)
{
    if(scale <= 0.0) return;
    if(scale > 15.0) {
        scale = 15.0;
    }
    this->scale = scale;
    this->scale -= (float)(qRound(this->scale * 100) % 5) / 100;
    this->repaint();
}

void Workspace::addScale(float diff)
{
    float f = this->scale + diff;
    if(f <= 0.0) return;
    if(f > 15.0) {
        f = 15.0;
    }
    this->scale = f;
    this->scale -= (float)(qRound(this->scale * 100) % 5) / 100;
    this->repaint();
}

float Workspace::getScale() const
{
    return this->scale;
}

void Workspace::mousePressEvent(QMouseEvent *event)
{
    switch (event->buttons()) {
    case Qt::LeftButton:
        // press event -> projekt
        if(this->project) {
            bool outOfArea = false;
            QPoint pos = this->calculateEventOffsetPosition(event->pos(), outOfArea);
            if(outOfArea) {
                this->project->outOfAreaEvent(pos);
            } else {
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
            bool outOfArea = false;
            QPoint pos = this->calculateEventOffsetPosition(event->pos(), outOfArea);
            if(outOfArea) {
                this->project->outOfAreaEvent(pos);
            } else {
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
            bool outOfArea = false;
            QPoint pos = this->calculateEventOffsetPosition(event->pos(), outOfArea);
            if(outOfArea) {
                this->project->outOfAreaEvent(pos);
            } else {
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
            bool outOfArea = false;
            QPoint pos = this->calculateEventOffsetPosition(event->pos(), outOfArea);
            if(outOfArea) {
                this->project->outOfAreaEvent(pos);
            } else {
                this->project->mouseMoveEvent(pos);
                this->repaint();
            }
        }
        break;
    case Qt::MiddleButton:
        // move event -> zmena offsetu workspace pomoci stredoveho tlacitka
        if(this->mouseHelper.processMoveEvent(event->pos())) {
            QPoint diff = this->mouseHelper.diffFromLastPos();
            this->globalOffset += diff * INV_SCALE(this->scale) * this->config.mouseSensitivity;
            this->repaint();
        }
        break;
    }
}

void Workspace::wheelEvent(QWheelEvent *event)
{
    // zoom in & zoom out
    if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier) == true) {
        if(event->angleDelta().y() > 0) {
            if(this->scale < 3) {
                this->addScale(0.05); // 5 %
            } else if(this->scale < 6) {
                this->addScale(0.2); // 20 %
            } else {
                this->addScale(0.4); // 40 %
            }
        } else {
            if(this->scale < 3) {
                this->addScale(-0.05);
            } else if(this->scale < 6) {
                this->addScale(-0.2);
            } else {
                this->addScale(-0.4);
            }
        }
    }
}

Tool *Workspace::getTool() const
{
    return tool;
}

void Workspace::setTool(Tool *newTool)
{
    if (tool == newTool)
        return;
    tool = newTool;
    emit toolChanged();
}

const Config_Workspace_t &Workspace::getConfig() const
{
    return this->config;
}

void Workspace::setConfig(const Config_Workspace_t &newConfig)
{
    this->config = newConfig;
    emit configChanged();
}

void Workspace::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    // nastaveni kvality vykreslovani
    painter.setRenderHint(QPainter::Antialiasing, true);

    // vykresleni pozadi
    painter.fillRect(this->rect(), QBrush(QColor(37, 37, 37), Qt::SolidPattern));


    if(this->project != NULL) {
        QSize s = this->project->getSize();

        // vypocet offsetu na stred celkove plochy workspacu
        QPoint offset(
                    (this->width() - s.width() * this->scale) / 2  + this->globalOffset.x() * this->scale,
                    (this->height() - s.height() * this->scale) / 2 + this->globalOffset.y() * this->scale
                    );

        //-------PROJECT-------------------------
        painter.save();
        // aplikace offsetu
        painter.translate(offset);
        // vykresleni pozadi obrazku (sachovnice)
        Layer_paintBgGrid(painter, s * this->scale, 15);
        // aplikace scale
        painter.scale(this->scale, this->scale);
        // vykresleni projektu
        this->project->paintEvent(painter);
        // vraceni zpet do puvodniho stavu
        painter.restore();
        //-------PROJECT-------------------------


        // vykresleni ramecku meritek
        painter.fillRect(26, 0, this->width(), 26, QBrush(QColor(50, 50, 50), Qt::SolidPattern));
        painter.fillRect(0, 26, 26, this->height(), QBrush(QColor(50, 50, 50), Qt::SolidPattern));
        painter.setFont(this->config.font);
        painter.setPen(QColor(150, 150, 150));


        // x osa meritko
        int scaled_size = s.width() * this->scale;
        int step = RULE_STEP_PX_MIN;
        float sf;
        for(int d = 4; d < 20; ++d)
        {
            sf = (float) scaled_size / d;
            if(sf < RULE_STEP_PX_MIN) {
                break;
            }
            if(scaled_size % d == 0) {
                step = sf;
            }
        }
        float px_step = s.width() / ((float)scaled_size / step);
        float from_start_to_0 = qCeil(offset.x() / step);
        float px = -px_step * from_start_to_0;
        for(int x = offset.x() - step * from_start_to_0;
            x < this->width();
            x+= step, px += px_step) {
            painter.drawText(QPointF(x + 5, 18), QString::number(px, 'f', 0));
            painter.drawLine(x, 4, x, 22);
        }


        // y osa meritko
        scaled_size = s.height() * this->scale;
        step = RULE_STEP_PX_MIN;
        for(int d = 2; d < 20; ++d)
        {
            sf = (float) scaled_size / d;
            if(sf < RULE_STEP_PX_MIN) {
                break;
            }
            if(scaled_size % d == 0) {
                step = sf;
            }
        }
        px_step = s.height() / ((float)scaled_size / step);
        from_start_to_0 = qCeil((float)offset.y() / step);
        QFontMetrics fm(this->config.font);
        px = -px_step * from_start_to_0;
        for(int y = offset.y() - step * from_start_to_0;
            y < this->height();
            y+= step, px += px_step) {

            QString num = QString::number(px, 'f', 0);
            int i = 0;
            for(QChar &c : num) {
                painter.drawText(QPointF(8, y + (i + 0.9) * (fm.height() - 4) + 5), c);
                ++i;
            }
            painter.drawLine(4, y, 22, y);
        }

        // stredovy ramecek meritek
        painter.fillRect(0, 0, 26, 26, QBrush(QColor(46, 46, 46), Qt::SolidPattern));


        // pozicni informace
        painter.fillRect(26, this->height() - 26, this->width(), 26, QBrush(QColor(45, 45, 45), Qt::SolidPattern));
        painter.setPen(QColor(210, 150, 150));
        Layer *l = this->project->getSelectedLayer();
        QString buffer = "Name: ";
        if(l) buffer += l->getName();
        painter.drawText(QPointF(
                             this->width() - 300,
                             this->height() - 9),
                         buffer);
        buffer = QString::number(this->scale * 100, 'f', 0) + "%";
        painter.drawText(QPointF(
                             this->width() - 170,
                             this->height() - 9),
                         buffer);
        bool b;
        QPoint pos = this->calculateEventOffsetPosition(this->currentPos, b);
        buffer = "X: " + QString::number(pos.x()) + " Y: " + QString::number(pos.y());
        painter.drawText(QPointF(
                             this->width() - 110,
                             this->height() - 9),
                         buffer);
    }

}

QPoint Workspace::calculateEventOffsetPosition(const QPoint &pos, bool &outOfRange) const
{
    // workspace center offset : (widget.size - project.size) / 2
    QSize s = this->project->getSize();
    QPoint offset(-(this->width() - s.width() * this->scale) / 2,
                  -(this->height() - s.height() * this->scale) / 2);

    // mouse event offset
    offset += pos;

    // global offset
    offset -= this->globalOffset * this->scale;

    // vynasobit inverznim scale
    // nyni uz finalni pozice prepoctana na soradnice v projektu
    offset *= INV_SCALE(this->scale);

    // pokud je mimo kreslici plochu tak zneplatni hodnoty
    if(offset.x() < 0 || offset.y() < 0 || offset.x() > s.width() || offset.y() > s.height()) {
        outOfRange = true;
    } else {
        outOfRange = false;
    }

    return offset;
}

Config_Workspace_t Workspace_defaultConfig()
{
    Config_Workspace_t cnfg;

    cnfg.font.setFamily("Monospace");
    cnfg.font.setPixelSize(11);
    cnfg.font.setStyle(QFont::StyleNormal);

    cnfg.mouseSensitivity = 1.3;

    return cnfg;
}
