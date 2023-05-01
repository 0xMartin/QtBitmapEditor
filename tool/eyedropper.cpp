#include "eyedropper.h"

#include <QVBoxLayout>
#include <QPainterPath>

#include "../utility/coloritem.h"


EyeDropperTool::EyeDropperTool(QObject *parent, ColorPicker *colorPicker) : Tool(parent)
{
    this->name = tr("EYEDROPPER");
    this->colorPicker = colorPicker; 
    this->setMouseTracking(true);

    //****************************************************************************************
    // sestaveni UI pro ovladani nastroje
    //****************************************************************************************
    this->ui = new QWidget();
    this->layout = new QVBoxLayout(this->ui);
    this->ui->setLayout(this->layout);

    // list s barvama
    this->listWidget = new QListWidget(this->ui);
    this->layout->addWidget(this->listWidget);
}

EyeDropperTool::~EyeDropperTool()
{
    if(this->layout) delete this->layout;
    if(this->listWidget) {
        this->listWidget->clear();
        delete this->listWidget;
    }
}

void EyeDropperTool::updatTool(float scale)
{
    this->updateTime = time(NULL);
    this->buffer = QImage(this->project->getSize(), QImage::Format_ARGB32);
    QPainter painter;
    painter.begin(&this->buffer);
    this->project->paintEvent(painter);
    painter.end();
}

void EyeDropperTool::paintEvent(const QPointF &pos, float scale, QPainter &painter)
{
    const int radius = 70;

    // aktualni zvolena brava
    painter.setPen(QPen(this->colorPicker->getColor(),
                        30, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
    painter.drawArc(
                pos.x() - radius,
                pos.y() - radius,
                radius * 2,
                radius * 2,
                180 * 16,
                180 * 16
                );

    // barva aktualniho pixelu
    QColor current = Qt::black;
    if(this->buffer.rect().contains(this->pos)) {
        current = QColor(this->buffer.pixel(this->pos));
    }
    painter.setPen(QPen(current, 30, Qt::SolidLine, Qt::RoundCap, Qt::MiterJoin));
    painter.drawArc(
                pos.x() - radius,
                pos.y() - radius,
                radius * 2,
                radius * 2,
                0,
                180 * 16
                );

    // outline
    painter.setBrush(Qt::transparent);
    painter.setPen(Qt::black);
    painter.drawEllipse(
                pos.x() - radius - 15,
                pos.y() - radius - 15,
                radius * 2 + 30,
                radius * 2 + 30
                );
    painter.drawEllipse(
                pos.x() - radius + 15,
                pos.y() - radius + 15,
                radius * 2 - 30,
                radius * 2 - 30
                );

    // hex a rgb kod
    QFontMetrics fm(painter.font());
    QString buffer = QString("RGB %1 %2 %3").arg(
                QString::number(current.red()),
                QString::number(current.green()),
                QString::number(current.blue()));
    QRect rect = fm.boundingRect(buffer);
    painter.fillRect(
                pos.x() - rect.width() / 2 - 10,
                pos.y() - radius - rect.height() * 2 - 10,
                rect.width() + 20,
                rect.height() * 2 + 10,
                Qt::black
                );
    painter.setPen(Qt::white);
    painter.drawText(QPointF(pos.x() - rect.width()/2, pos.y() - radius - rect.height() - 10), buffer);
    buffer = current.name().toUpper();
    rect = fm.boundingRect(buffer);
    painter.drawText(QPointF(pos.x() - rect.width()/2, pos.y() - radius - 10), buffer);
}

bool EyeDropperTool::overLayerPainting() const
{
    return true;
}

int EyeDropperTool::getType() const
{
    return TOOL_EYEDROPPER;
}


/*****************************************************************************************/
// EVENTY PRO EDITACI BITMAPY
/*****************************************************************************************/

void EyeDropperTool::mousePressEvent(const QPointF &pos)
{
    if(this->colorPicker != NULL) {
        if(this->buffer.rect().contains(this->pos)) {
            QColor c = QColor(this->buffer.pixel(this->pos));
            // nastavy barvu do externiho color pickeru
            this->colorPicker->setColor(c);
            // prida tuto barvu i do listu
            const QSize size = QSize(50, 45);
            ColorItem *cItem = new ColorItem(c, this->colorPicker, size, this->ui);
            QListWidgetItem *item = new QListWidgetItem();
            item->setSizeHint(size);
            this->listWidget->insertItem(0, item);
            this->listWidget->setItemWidget(item, cItem);
            this->listWidget->repaint();
        }
    }
}

void EyeDropperTool::mouseReleaseEvent(const QPointF &pos)
{
}

void EyeDropperTool::mouseDoubleClickEvent(const QPointF &pos)
{
}

void EyeDropperTool::mouseMoveEvent(const QPointF &pos)
{
    // ulozeni posledni aktualni pozice
    this->pos = QPoint(pos.x(), pos.y());

    // kazdou sekundu update pokud dojde k pohybu
    time_t current = time(NULL);
    if(current - this->updateTime > 1) {
        this->updatTool(1.0);
    }
}

void EyeDropperTool::outOfAreaEvent(const QPointF &pos)
{
}

