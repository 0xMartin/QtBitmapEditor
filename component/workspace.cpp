#include "workspace.h"

#include <QPainter>
#include <QtMath>

Workspace::Workspace(QWidget *parent): QWidget(parent)
{
    this->setBackgroundRole(QPalette::Base);
    this->setAutoFillBackground(true);
    this->setMinimumSize(QSize(400, 400));
    this->project = NULL;
    this->scale = 1.0f;
}

Workspace::~Workspace() {
    if(this->project) delete project;
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

void Workspace::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    // nastaveni kvality vykreslovani
    painter.setRenderHint(QPainter::Antialiasing, true);

    // update velikosti
    updateSizeOfWorkaspace();

    // vykresleni pozadi workspace (sachovnice)
    painter.fillRect(this->rect(), QBrush(Qt::white, Qt::SolidPattern));
    int step = this->width() / 40;
    int step2 = 2 * step;
    QBrush brush(QColor(200, 200, 200), Qt::SolidPattern);
    for(int y = 0, i = 0; y < this->height(); y += step, ++i) {
        int x_offset = i % 2 == 0 ? 0 : step;
        for(int x = 0; x < this->width(); x += step2) {
            painter.fillRect(x + x_offset, y, step, step, brush);
        }
    }

    // vykresleni projektu
    if(this->project != NULL) {
        // vypocet offsetu na stred celkove plochy workspacu
        QSize s = this->project->getSize();
        QPoint offset((this->width() - s.width()) / 2, (this->height() - s.height()) / 2);
        // vykresleni
        this->project->paintEvent(painter, offset);
    }

    // vykresleni pravitka

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
