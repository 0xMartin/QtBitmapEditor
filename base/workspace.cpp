#include "workspace.h"

#include "config.h"

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
    this->mouseHelper = MouseEventHelper(3);
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
    // update scale
    if(scale <= 0.0) return;
    if(scale > 40.0) {
        scale = 40.0;
    }
    this->scale = scale;
    this->scale -= (float)(qRound(this->scale * 100) % 5) / 100;
    // update scale nastroje
    if(this->tool) {
        this->tool->updateScale(this->scale);
    }
}

void Workspace::addScale(float diff)
{
    // update scale
    float f = this->scale + diff;
    if(f <= 0.0) return;
    if(f > 40.0 ) {
        f = 40.0;
    }
    this->scale = f;
    this->scale -= (float)(qRound(this->scale * 100) % 5) / 100;
    // update mouse helper
    this->mouseHelper.updateDistance(DEFAULT_MOUSE_HELPER_DIST * INV_SCALE(this->scale));
    // update scale nastroje
    if(this->tool) {
        this->tool->updateScale(this->scale);
    }
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
            QPointF pos = this->calculateEventOffsetPosition(event->pos(), outOfArea);
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
            QPointF pos = this->calculateEventOffsetPosition(event->pos(), outOfArea);
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
            QPointF pos = this->calculateEventOffsetPosition(event->pos(), outOfArea);
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
            QPointF pos = this->calculateEventOffsetPosition(event->pos(), outOfArea);
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
            QPointF diff = this->mouseHelper.diffFromLastPos();
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
        int diff = (event->angleDelta().y() > 0 ? 1 : -1) * this->width() / 14 * INV_SCALE(this->scale);
        int x = this->globalOffset.x() + diff;
        // TODO ... x limits
        this->globalOffset.setX(x);
    } else {
        // posun ve vertikalni ose
        int diff = (event->angleDelta().y() > 0 ? 1 : -1) * this->height() / 14 * INV_SCALE(this->scale);
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
    // prida novy nastroj
    if (tool == newTool)
        return;
    tool = newTool;
    emit toolChanged();

    // update scale nastroje
    if(this->tool) {
        this->tool->updateScale(this->scale);
    }
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
        float tile_size = this->scale >= 15 ? this->scale : 15; /** Velikost policka = 15 a pokud se zobrazi pixel grid zmeni velikost na hodnotu scale */
        Layer_paintBgGrid(painter, offset, this->size(), s * this->scale, tile_size);


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
        // outline
        painter.setPen(Qt::black);
        painter.drawRect(offset.x(), offset.y(), s.width() * this->scale, s.height() * this->scale);
        //-------PROJECT-------------------------


        painter.setRenderHint(QPainter::Antialiasing, true);

        // overlayer
        if(this->tool != NULL) {
            this->tool->paintEvent(this->currentPos, this->scale, painter);
        }


        // pixel grid (jen kdyz je obrazek hodne priblizen)
        if(this->scale >= PIXEL_GRID_MIN_SCALE) {
            painter.setPen(QColor(100, 100, 100));
            int s_w = s.width() * this->scale;
            int s_h = s.height() * this->scale;
            for(float x = 0; (int)x < s_w; x += this->scale) {
                painter.drawLine(
                            offset.x() + x,
                            offset.y(),
                            offset.x() + x,
                            offset.y() + s_h);
            }
            for(float y = 0; (int)y < s_h; y += this->scale) {
                painter.drawLine(
                            offset.x(),
                            offset.y() + y,
                            offset.x() + s_w,
                            offset.y() + y);
            }
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
        QPointF pos = this->calculateEventOffsetPosition(this->currentPos, b);
        buffer += "X: " + QString::number(pos.x(), 'f', 0) + " Y: " + QString::number(pos.y(), 'f', 0);
        buffer = buffer.leftJustified(51, ' ');
        // dx & dy
        pos = pos - this->pressPos;
        buffer += "DX: " + QString::number(pos.x(), 'f', 0) + " DY: " + QString::number(pos.y(), 'f', 0);
        // paint info
        painter.drawText(QPointF(
                             40,
                             this->height() - 9),
                         buffer);
    }

}

QPointF Workspace::calculateEventOffsetPosition(const QPointF &pos, bool &outOfRange) const
{
    // workspace center offset : (widget.size - project.size) / 2
    QSize s = this->project->getSize();
    QPointF offset(-(this->width() - s.width() * this->scale) / 2,
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
