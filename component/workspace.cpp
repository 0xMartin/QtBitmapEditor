#include "workspace.h"

#include <QPainter>
#include <QtMath>

// velikost useku meritka v pixelech
#define RULE_STEP_PX 100


Workspace::Workspace(QWidget *parent): QWidget(parent)
{
    this->project = NULL;
    this->scale = 1.0f;

    this->setMinimumSize(QSize(400, 400));
    this->setBackgroundRole(QPalette::Base);
    this->setAutoFillBackground(true);

    this->font.setFamily("Monospace");
    this->font.setPixelSize(11);
    this->font.setStyle(QFont::StyleNormal);
}

void Workspace::setProject(Project *project) {
    this->project = project;
    this->repaint();
}

Project *Workspace::getProject() const {
    return this->project;
}

void Workspace::setScale(float scale)
{
    this->scale = scale;
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
        QPoint offset((this->width() - s.width()) / 2, (this->height() - s.height()) / 2);

        // vykresleni projektu
        this->project->paintEvent(painter, offset);

        // vykresleni ramecku meritek
        painter.fillRect(26, 0, this->width(), 26, QBrush(QColor(50, 50, 50), Qt::SolidPattern));
        painter.fillRect(0, 26, 26, this->height(), QBrush(QColor(50, 50, 50), Qt::SolidPattern));

        painter.setFont(this->font);
        painter.setPen(QColor(150, 150, 150));

        // x osa
        int parts = qRound((float)s.width() / RULE_STEP_PX);
        int px_step = s.width() / parts;
        int step = px_step;
        int from_start_to_0 = qCeil(offset.x() / RULE_STEP_PX);
        for(int x = offset.x() - step * from_start_to_0, px = -px_step * from_start_to_0; x < this->width(); x+= step, px += px_step) {
            painter.drawText(QPointF(x, 18), QString::number(px));
            painter.drawLine(x - 5, 4, x - 5, 22);
        }

        // y osa
        parts = qRound((float)s.height() / RULE_STEP_PX);
        px_step = s.height() / parts;
        step = px_step;
        from_start_to_0 = qCeil((float)offset.y() / RULE_STEP_PX);
        QFontMetrics fm(this->font);
        for(int y = offset.y() - step * from_start_to_0, px = -px_step * from_start_to_0; y < this->height(); y+= step, px += px_step) {
            QString num = QString::number(px);
            int i = 0;
            for(QChar &c : num) {
                painter.drawText(QPointF(8, y + (i + 0.9) * (fm.height() - 4)), c);
                ++i;
            }
            painter.drawLine(4, y - 5, 22, y - 5);
        }

        // stredovy ramecek meritek
        painter.fillRect(0, 0, 26, 26, QBrush(QColor(45, 45, 45), Qt::SolidPattern));
    }

}

void Workspace::updateSizeOfWorkaspace() {
    QSize size = this->parentWidget()->size();

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
