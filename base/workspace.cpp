#include "workspace.h"


#include <chrono>
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
    this->tool = NULL;
    this->scale = 1.0f;
    this->mouseHelper = MouseEventHelper(5);
    this->globalOffset = QPoint(0, 0);
    this->currentPos = QPoint(0, 0);
    this->pressPos = QPoint(0, 0);
    this->begin = std::chrono::steady_clock::now();

    this->setMinimumSize(QSize(400, 400));
    this->setBackgroundRole(QPalette::Base);
    this->setAutoFillBackground(true);
    this->setMouseTracking(true);
}

void Workspace::setProject(Project *project) {
    this->project = project;
    this->project->setParent(this);

    // vypocet vhodneho meritka pri prvnim nacteni projektu
    if(this->project->getSize().width() > this->project->getSize().height()) {
        this->setScale(this->width() * 0.8 / this->project->getSize().width());
    } else {
        this->setScale(this->scale = this->height() * 0.8 / this->project->getSize().height());
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
}

void Workspace::zoomIN()
{
    if(this->scale < 3) {
        this->addScale(0.05); // 5 %
    } else if(this->scale < 6) {
        this->addScale(0.2); // 20 %
    } else {
        this->addScale(0.4); // 40 %
    }
}

void Workspace::zoomOUT()
{
    if(this->scale < 3) {
        this->addScale(-0.05);
    } else if(this->scale < 6) {
        this->addScale(-0.2);
    } else {
        this->addScale(-0.4);
    }
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
        if(this->tool) {
            bool outOfArea = false;
            QPoint pos = this->calculateEventOffsetPosition(event->pos(), outOfArea);
            this->pressPos = pos;
            if(outOfArea) {
                this->tool->outOfAreaEvent(pos);
            } else {
                this->tool->mousePressEvent(pos);
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
        if(this->tool) {
            bool outOfArea = false;
            QPoint pos = this->calculateEventOffsetPosition(event->pos(), outOfArea);
            if(outOfArea) {
                this->tool->outOfAreaEvent(pos);
            } else {
                this->tool->mouseReleaseEvent(pos);
                this->repaint();
            }
        }

        // repaint signal
        emit this->project->repaintSignal(this->project->getSelectedLayer());
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
        if(this->tool) {
            bool outOfArea = false;
            QPoint pos = this->calculateEventOffsetPosition(event->pos(), outOfArea);
            if(outOfArea) {
                this->tool->outOfAreaEvent(pos);
            } else {
                this->tool->mouseDoubleClickEvent(pos);
                this->repaint();
            }
        }
        break;
    }
}

void Workspace::mouseMoveEvent(QMouseEvent *event)
{
    this->currentPos = event->pos();

    bool paint = false;

    switch (event->buttons()) {
    case Qt::LeftButton:
        // press event -> projekt
        if(this->tool) {
            bool outOfArea = false;
            QPoint pos = this->calculateEventOffsetPosition(event->pos(), outOfArea);
            if(outOfArea) {
                this->tool->outOfAreaEvent(pos);
            } else {
                this->tool->mouseMoveEvent(pos);
                paint = true;
            }
        }
        break;
    case Qt::MiddleButton:
        // move event -> zmena offsetu workspace pomoci stredoveho tlacitka
        if(this->mouseHelper.processMoveEvent(event->pos())) {
            QPoint diff = this->mouseHelper.diffFromLastPos();
            this->globalOffset += diff * INV_SCALE(this->scale) * this->config.mouseSensitivity;
            paint = true;
        }
        break;
    }

    if(this->tool != NULL) {
        if(this->tool->overLayerPainting()) {
            paint = true;
        }
    }

    // jednotne vykreslovani (limitovano na max 50 fps)
    if(paint) {
        // fps limitation
        std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
        if(std::chrono::duration_cast<std::chrono::milliseconds>(now - begin).count() > 1000.0 / this->config.fps) {
            begin = now;
            this->repaint();
        }
    }
}

void Workspace::wheelEvent(QWheelEvent *event)
{
    if (QApplication::keyboardModifiers().testFlag(Qt::AltModifier) == true) {
        // zoom in & zoom out
        if(event->angleDelta().y() > 0) {
            this->zoomIN();
        } else {
            this->zoomOUT();
        }
    } else if (QApplication::keyboardModifiers().testFlag(Qt::ControlModifier) == true) {
        // posun v horizontalni ose
        int diff = (event->angleDelta().y() > 0 ? 1 : -1) * this->width() / 12 * INV_SCALE(this->scale);
        int x = this->globalOffset.x() + diff;
        // TODO ... x limits
        this->globalOffset.setX(x);
    } else {
        // posun ve vertikalni ose
        int diff = (event->angleDelta().y() > 0 ? 1 : -1) * this->height() / 12 * INV_SCALE(this->scale);
        int y = this->globalOffset.y() + diff;
        // TODO ... y limits
        this->globalOffset.setY(y);
    }

    // jednotne vykreslovani (limitovano na max 50 fps)
    // fps limitation
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    if(std::chrono::duration_cast<std::chrono::milliseconds>(now - begin).count() > 1000.0 / this->config.fps) {
        begin = now;
        this->repaint();
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


        // vykresleni pozadi obrazku (sachovnice)
        Layer_paintBgGrid(painter, offset, this->size(), s * this->scale, 15);


        //-------PROJECT-------------------------
        painter.save();
        // aplikace offsetu
        painter.translate(offset);
        // aplikace scale
        painter.scale(this->scale, this->scale);
        // vykresleni projektu
        this->project->paintEvent(painter);
        // vraceni zpet do puvodniho stavu
        painter.restore();
        //-------PROJECT-------------------------


        painter.setRenderHint(QPainter::Antialiasing, true);

        // overlayer
        if(this->tool != NULL) {
            this->tool->paintEvent(this->currentPos, this->scale, painter);
        }


        // vykresleni ramecku meritek
        painter.fillRect(26, 0, this->width(), 26, QBrush(QColor(50, 50, 50), Qt::SolidPattern));
        painter.fillRect(0, 26, 26, this->height(), QBrush(QColor(50, 50, 50), Qt::SolidPattern));
        painter.setFont(this->config.font);
        painter.setPen(QColor(150, 150, 150));


        // x osa meritko
        int scaled_size = s.width() * this->scale;
        float step = RULE_STEP_PX_MIN;
        float sf;
        for(int d = 1; d <= 20; ++d)
        {
            sf = (float) scaled_size / d;
            if(sf < RULE_STEP_PX_MIN) {
                break;
            }
            if(s.width() % d == 0) {
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
        for(int d = 1; d <= 20; ++d)
        {
            sf = (float) scaled_size / d;
            if(sf < RULE_STEP_PX_MIN) {
                break;
            }
            if(s.height() % d == 0) {
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
        QString buffer = "";
        // jmeno vrstvy
        Layer *l = this->project->getSelectedLayer();
        if(l) buffer += l->getName().mid(0, qMin(l->getName().length(), 18));
        buffer = buffer.leftJustified(20, ' ');
        // zoom
        buffer += QString::number(this->scale * 100, 'f', 0) + "%";
        buffer = buffer.leftJustified(31, ' ');
        // pozice
        bool b;
        QPoint pos = this->calculateEventOffsetPosition(this->currentPos, b);
        buffer += "X: " + QString::number(pos.x()) + " Y: " + QString::number(pos.y());
        buffer = buffer.leftJustified(51, ' ');
        // dx & dy
        pos = pos - this->pressPos;
        buffer += "DX: " + QString::number(pos.x()) + " DY: " + QString::number(pos.y());
        // paint info
        painter.drawText(QPointF(
                             40,
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
    cnfg.fps = 50;

    cnfg.mouseSensitivity = 1.3;

    return cnfg;
}
