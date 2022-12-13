#include "brush.h"

#include <QVBoxLayout>
#include <QPainterPath>

#include "../base/config.h"
#include "../layer/bitmaplayer.h"


Brush::Brush(QObject *parent, ColorPicker *colorPicker) : Tool(parent)
{
    this->name = tr("BRUSH");
    this->mouseHelper = MouseEventHelper(DEFAULT_MOUSE_HELPER_DIST);
    this->colorPicker = colorPicker;

    //****************************************************************************************
    // sestaveni UI pro ovladani nastroje
    //****************************************************************************************
    this->layout = new QVBoxLayout(this->ui);
    this->ui->setLayout(this->layout);

    // velikost stetce
    this->spinbox_size = new QSpinBox();
    this->spinbox_size->setPrefix(tr("Brush Size:"));
    this->spinbox_size->setSuffix("px");
    this->spinbox_size->setMinimum(MIN_TOOL_SIZE);
    this->spinbox_size->setMaximum(MAX_TOOL_SIZE);
    this->spinbox_size->setValue(DEFAULT_TOOL_SIZE);
    this->layout->addWidget(this->spinbox_size);

    // tvar
    this->comboBox_shape = new QComboBox();
    this->comboBox_shape->addItem(tr("Solid"));
    this->comboBox_shape->addItem(tr("Dense 1"));
    this->comboBox_shape->addItem(tr("Dense 2"));
    this->comboBox_shape->addItem(tr("Dense 3"));
    this->comboBox_shape->addItem(tr("Dense 4"));
    this->comboBox_shape->addItem(tr("Dense 5"));
    this->comboBox_shape->addItem(tr("Dense 6"));
    this->comboBox_shape->addItem(tr("Horizontal"));
    this->comboBox_shape->addItem(tr("Vertical"));
    this->comboBox_shape->addItem(tr("Cross"));
    this->comboBox_shape->addItem(tr("Diagonal 1"));
    this->comboBox_shape->addItem(tr("Diagonal 2"));
    this->comboBox_shape->addItem(tr("Diagonal Cross"));
    this->comboBox_shape->addItem(tr("Linear Gradient"));
    this->layout->addWidget(this->comboBox_shape);

    // nahled tvaru
    this->label_shape = new QLabel();
    this->layout->addWidget(this->label_shape);

    // gradient
    this->gradientEditor = new GradientEditor();
    this->gradientEditor->setVisible(false);
    this->layout->addWidget(this->gradientEditor);

    this->on_comboBox_shape_changed(0);
    connect(this->comboBox_shape, SIGNAL(currentIndexChanged(int)),
            this, SLOT(on_comboBox_shape_changed(int)));
    connect(this->colorPicker, SIGNAL(colorChange(QColor)),
            this, SLOT(on_color_changed(QColor)));

    // antialiasing
    this->checkBox_Antialiasing = new QCheckBox();
    this->checkBox_Antialiasing->setChecked(true);
    this->checkBox_Antialiasing->setText(tr("Smooth Painting Mode"));
    this->layout->addWidget(this->checkBox_Antialiasing);

    // spacer
    this->layout->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Fixed, QSizePolicy::Expanding));
}

Brush::~Brush()
{
    if(this->layout) delete this->layout;
    if(this->comboBox_shape) delete this->comboBox_shape;
    if(this->label_shape) delete this->label_shape;
    if(this->gradientEditor) delete this->gradientEditor;
    if(this->spinbox_size) delete this->spinbox_size;
    if(this->checkBox_Antialiasing) delete this->checkBox_Antialiasing;
}

void Brush::updatTool(float scale)
{
    // update mouse helper
    this->mouseHelper.updateDistance(mapFunc(scale, 1.0, PIXEL_GRID_MIN_SCALE, DEFAULT_MOUSE_HELPER_DIST, 1.0));

    // update brush
    this->updateBrush(this->brush);
}

void Brush::paintEvent(const QPointF &pos, float scale, QPainter &painter)
{
    // vykresleni tvaru a veliskota nastroje do horni nahledove vrstvy
    int size = this->spinbox_size->value();
    painter.setPen(Qt::black);
    float s = size * scale;
    painter.drawEllipse(pos.x() - s/2, pos.y() - s/2, s, s);
}

bool Brush::overLayerPainting() const
{
    return true;
}

int Brush::getType() const
{
    return TOOL_BRUSH;
}


/*****************************************************************************************/
// EVENTY PRO EDITACI BITMAPY
/*****************************************************************************************/

void Brush::mousePressEvent(const QPointF &pos)
{
    this->mouseHelper.processMoveEvent(pos);

    // vykreleni po dotiku
    int size = this->spinbox_size->value();
    BitmapLayer *layer = (BitmapLayer *)this->layerCheck(BITMAP_LAYER_TYPE);
    if(layer == NULL) return;
    this->painter.begin(&layer->image);
    painter.setRenderHint(QPainter::Antialiasing, this->checkBox_Antialiasing->isChecked());
    this->painter.setBrush(this->brush);
    this->painter.setPen(Qt::transparent);
    this->painter.drawEllipse(pos.x() - size/2, pos.y() - size/2, size, size);
    this->painter.end();
}

void Brush::mouseReleaseEvent(const QPointF &pos)
{
    this->mouseHelper.resetMove();
}

void Brush::mouseDoubleClickEvent(const QPointF &pos)
{
}

void Brush::mouseMoveEvent(const QPointF &pos)
{
    if(this->mouseHelper.processMoveEvent(pos)) {
        // po definovanych vzdalenost dela tah
        QLineF line = this->mouseHelper.lineFromLastPos();

        BitmapLayer *layer = (BitmapLayer *)this->layerCheck(BITMAP_LAYER_TYPE);
        if(layer == NULL) return;
        this->painter.begin(&layer->image);
        painter.setRenderHint(QPainter::Antialiasing, this->checkBox_Antialiasing->isChecked());
        this->painter.setBrush(this->brush);
        this->paintLineWithBrush(this->painter, line);
        this->painter.end();
    }
}

void Brush::outOfAreaEvent(const QPointF &pos)
{
    // dokonci tah
    const QPointF *last = this->mouseHelper.getLast();
    if(last) {
        QLineF line(*last, pos);

        BitmapLayer *layer = (BitmapLayer *)this->layerCheck(BITMAP_LAYER_TYPE);
        if(layer == NULL) return;
        this->painter.begin(&layer->image);
        painter.setRenderHint(QPainter::Antialiasing, this->checkBox_Antialiasing->isChecked());
        this->painter.setBrush(this->brush);
        this->paintLineWithBrush(this->painter, line);
        this->painter.end();

        // reset
        this->mouseHelper.resetMove();
        // request repain
        layer->requestRepaint();
    }
}


/*****************************************************************************************/
// PRIVATNI FUNKCE
/*****************************************************************************************/

void Brush::paintLineWithBrush(QPainter &painter, const QLineF &line)
{
    this->painter.setBrush(this->brush);
    this->painter.setPen(Qt::transparent);
    int size = this->spinbox_size->value();


    int x0 = line.p1().x();
    int y0 = line.p1().y();
    int x1 = line.p2().x();
    int y1 = line.p2().y();

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;
    int err = dx - dy;

    while (true) {
        this->painter.drawEllipse(x0 - size/2, y0 - size/2, size, size);

        if (x0 == x1 && y0 == y1) break;
        int e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void Brush::updateBrush(QBrush &b)
{
    switch (this->comboBox_shape->currentIndex()) {
    case 0: // Solid
        b = QBrush(this->colorPicker->getColor(), Qt::SolidPattern);
        break;
    case 1: // Dense 1
        b = QBrush(this->colorPicker->getColor(), Qt::Dense1Pattern);
        break;
    case 2: // Dense 2
        b = QBrush(this->colorPicker->getColor(), Qt::Dense2Pattern);
        break;
    case 3: // Dense 3
        b = QBrush(this->colorPicker->getColor(), Qt::Dense3Pattern);
        break;
    case 4: // Dense 4
        b = QBrush(this->colorPicker->getColor(), Qt::Dense4Pattern);
        break;
    case 5: // Dense 5
        b = QBrush(this->colorPicker->getColor(), Qt::Dense5Pattern);
        break;
    case 6: // Dense 6
        b = QBrush(this->colorPicker->getColor(), Qt::Dense6Pattern);
        break;
    case 7: // Horizontal
        b = QBrush(this->colorPicker->getColor(), Qt::HorPattern);
        break;
    case 8: // Vertical
        b = QBrush(this->colorPicker->getColor(), Qt::VerPattern);
        break;
    case 9: // Cross
        b = QBrush(this->colorPicker->getColor(), Qt::CrossPattern);
        break;
    case 10: // Diagonal 1
        b = QBrush(this->colorPicker->getColor(), Qt::BDiagPattern);
        break;
    case 11: // Diagonal 2
        b = QBrush(this->colorPicker->getColor(), Qt::FDiagPattern);
        break;
    case 12: // Diagonal Cross
        b = QBrush(this->colorPicker->getColor(), Qt::DiagCrossPattern);
        break;
    case 13: // Linear Gradient
        if(this->gradientEditor != NULL) {
            QLinearGradient g = this->gradientEditor->getAsLinearGradient(
                        0,
                        0,
                        this->project->getSize().width(),
                        0
                        );
            b = QBrush(g);
        }
        break;
    }
}

void Brush::on_comboBox_shape_changed(int index) {
    if(this->label_shape == NULL) return;

    // prekresli nahled stetce
    this->on_color_changed(this->colorPicker->getColor());

    // zobrazi / skryje gradient
    if(this->comboBox_shape->currentIndex() < 13) {
        this->gradientEditor->setVisible(false);
    } else {
        this->gradientEditor->setVisible(true);
    }
    this->ui->repaint();
}

void Brush::on_color_changed(const QColor &color)
{
    // prekresli nahled stetce
    QSize s = QSize(80, 80);
    this->label_shape->setFixedSize(s);
    QPixmap pixmap(s);
    pixmap.fill(Qt::transparent);
    QBrush b;
    this->updateBrush(b);
    QPainter p;
    p.begin(&pixmap);
    p.setRenderHint(QPainter::Antialiasing, true);
    p.setBrush(QBrush(Qt::white));
    p.drawEllipse(0, 0, s.width(), s.height());
    p.setBrush(b);
    p.drawEllipse(0, 0, s.width(), s.height());
    p.end();
    this->label_shape->setPixmap(pixmap);
}
