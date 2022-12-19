#include "project.h"

#include "config.h"

#include <QWidget>
#include <QPainter>
#include <QtMath>
#include <QFile>
#include <QFileInfo>

#include "layer.h"
#include "../layer/bitmaplayer.h"
#include "../layer/textlayer.h"
#include "../layer/imagelayer.h"


Project::Project(QObject *parent, const QString &name, const QString &path, const QSize &size) : QObject(parent)
{
    this->setName(name);
    this->setPath(path);
    this->setSize(size);
    this->selected_layer = NULL;
    this->mode = PROJECT_EDIT;
    this->layers = new Layers_t();
}

Project::~Project()
{
    if(this->layers) {
        for(Layer *l : *this->layers) {
            if(l) delete l;
        }
        delete this->layers;
    }
}

const QString &Project::getName() const
{
    return this->name;
}

bool Project::setName(const QString &name) {
    if(name.length() < PROJECT_MIN_NAME_LENGTH) return false;
    this->name = name;
    return true;
}

const QString &Project::getPath() const
{
    return this->path;
}

QString Project::getDirPath()
{
    QFileInfo fi(this->path);
    return fi.absolutePath() + '/';
}

void Project::setPath(const QString &path)
{
    this->path = path;
}

bool Project::setSize(const QSize &size) {
    if(size.width() < 1 || size.height() < 1) return false;
    this->size = size;
    return true;
}

const QSize &Project::getSize() const {
    return this->size;
}

Layers_t *Project::getLayers() const {
    return this->layers;
}

bool Project::addLayerAtTop(Layer *layer)
{
    if(layer == NULL) return false;
    if(this->layers == NULL) return false;

    this->layers->push_back(layer);
    emit this->layerListChanged();

    return true;
}

bool Project::insertLayer(qsizetype index, Layer *layer)
{
    if(layer == NULL) return false;
    if(this->layers == NULL) return false;

    this->layers->insert(index, layer);
    emit this->layerListChanged();

    return true;
}

bool Project::insertLayerAbove(Layer *layer)
{
    if(layer == NULL) return false;
    if(this->layers == NULL) return false;

    qsizetype index = 0;
    for(Layer *l : *this->layers) {
        if(l == this->selected_layer) {
            // prida novou vrstvu
            this->layers->insert(index + 1, layer);
            emit this->layerListChanged();
            return true;
        }
        ++index;
    }

    this->layers->push_back(layer);
    emit this->layerListChanged();
    return true;
}

bool Project::removeLayer(Layer *layer)
{
    if(layer == NULL) return false;
    if(this->layers == NULL) return false;
    if(this->layers->size() <= 1) return false;

    qsizetype index = 0;
    for(Layer *l : *this->layers) {
        if(l == layer) {
            // pokud odebrana vrstava byla v projektu oznacena tak zrusi oznaceni
            if(layer == this->selected_layer)
                this->setSelectedLayer(NULL);
            // odstrani vrstvu (delete + odebrani z listu)
            delete (this->layers->takeAt(index));
            emit this->layerListChanged();
            return true;
        }
        ++index;
    }

    return false;
}

bool Project::saveProject() const
{
    if(path.length() == 0) return false;

    QFile file(path);
    if(!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    file.close();

    return true;
}

Layer *Project::getSelectedLayer() const
{
    return this->selected_layer;
}

void Project::setSelectedLayer(Layer *newSelected_layer)
{
    this->selected_layer = newSelected_layer;
    emit this->selectedLayerChanged(this->selected_layer);
}

void Project::requestRepaint()
{
    QObject *parent = this->parent();
    if(parent) {
        ((QWidget*) parent)->repaint();
    }
}

bool Project::moveSelectedLayerUp()
{
    if(this->selected_layer == NULL) return false;
    qsizetype index = 0;
    for(Layer *l : *this->layers) {
        if(l == this->selected_layer) {
            if(index + 1 < this->layers->size()) {
                Layer *layer = this->layers->takeAt(index);
                this->layers->insert(index + 1, layer);
                return true;
            }
        }
        ++index;
    }
    return false;
}

bool Project::moveSelectedLayerDown()
{
    if(this->selected_layer == NULL) return false;
    qsizetype index = 0;
    for(Layer *l : *this->layers) {
        if(l == this->selected_layer) {
            if(index > 0) {
                Layer *layer = this->layers->takeAt(index);
                this->layers->insert(index - 1, layer);
                return true;
            }
        }
        ++index;
    }
    return true;
}

bool Project::mergaSeletedLayerDown()
{
    if(this->selected_layer == NULL) return false;
    if(this->layers->length() < 2) return false;

    qsizetype index = 0;
    for(Layer *l : *this->layers) {
        if(l == this->selected_layer) {
            if(index > 0) {
                if(this->layers->at(index - 1)->getType() != BITMAP_LAYER_TYPE) return false;
                BitmapLayer *buttom = (BitmapLayer *)this->layers->at(index - 1);
                Layer *top = this->layers->takeAt(index);

                // image merge
                QPainter painter;
                painter.begin(&buttom->image);
                top->paintEvent(painter);
                painter.end();

                // mask merge
                if(top->getMask() != NULL) {
                    if(buttom->getMask() == NULL) buttom->createMask();
                    buttom->maskPaste(top->getMask());
                }

                this->setSelectedLayer(buttom);

                delete top;
                return true;
            }
        }
        ++index;
    }

    return true;
}

bool Project::dupliceteLayer()
{
    if(this->selected_layer == NULL) return false;
    qsizetype index = 0;
    for(Layer *l : *this->layers) {
        if(l == this->selected_layer) {
            Layer *duplicate = this->selected_layer->createDuplicate();
            if(duplicate == NULL) {
                return false;
            }
            this->layers->insert(index + 1, duplicate);
            return true;
        }
        ++index;
    }
    return true;
}

bool Project::rasterizeLayer()
{
    if(this->selected_layer == NULL) return false;
    if(this->selected_layer->getType() == BITMAP_LAYER_TYPE) return false;

    qsizetype index = 0;
    for(Layer *l : *this->layers) {
        if(l == this->selected_layer) {
            Layer *layer = this->layers->takeAt(index);
            if(layer == NULL) return false;

            BitmapLayer *bitmapLayer = ResterizeLayer(layer);
            this->setSelectedLayer(bitmapLayer);
            this->layers->insert(index, bitmapLayer);

            delete layer;
            return true;
        }
        ++index;
    }

    return false;
}

ProjectEditMode_t Project::getMode() const
{
    return this->mode;
}

void Project::setMode(ProjectEditMode_t newMode)
{
    this->mode = newMode;
}

void Project::paintEvent(QPainter &painter) {
    if(this->layers == NULL) return;

    switch (this->mode) {
    case PROJECT_EDIT:
        // vykresli postupne vsechny vrstvy projektu (normalni pracovni mod)
        for(Layer *layer : *this->layers) {
            if(layer != NULL) {
                if(!layer->isVisible()) continue;
                // ulozeni aktualniho stavu
                painter.save();
                // antialiasing
                painter.setRenderHint(QPainter::Antialiasing, layer->isAntialiasingEnabled());
                // opacity
                painter.setOpacity(layer->getOpacity());
                // blend mode
                switch (layer->getBlendMode()) {
                case NORMAL:
                    painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
                    break;
                case ADDITION:
                    painter.setCompositionMode(QPainter::CompositionMode_Plus);
                    break;
                case MULTIPLY:
                    painter.setCompositionMode(QPainter::CompositionMode_Multiply);
                    break;
                case SCREEN:
                    painter.setCompositionMode(QPainter::CompositionMode_Screen);
                    break;
                case OVERLAY:
                    painter.setCompositionMode(QPainter::CompositionMode_Overlay);
                    break;
                case DARKEN:
                    painter.setCompositionMode(QPainter::CompositionMode_Darken);
                    break;
                case LIGHTEN:
                    painter.setCompositionMode(QPainter::CompositionMode_Lighten);
                    break;
                case COLOR_DODGE:
                    painter.setCompositionMode(QPainter::CompositionMode_ColorDodge);
                    break;
                case COLOR_BURN:
                    painter.setCompositionMode(QPainter::CompositionMode_ColorBurn);
                    break;
                case HARD_LIGHT:
                    painter.setCompositionMode(QPainter::CompositionMode_HardLight);
                    break;
                case SOFT_LIGHT:
                    painter.setCompositionMode(QPainter::CompositionMode_SoftLight);
                    break;
                case DIFFERENCE:
                    painter.setCompositionMode(QPainter::CompositionMode_Difference);
                    break;
                case EXCLUSION:
                    painter.setCompositionMode(QPainter::CompositionMode_Exclusion);
                    break;
                }
                // aplikuje masku vrtvy pokud existuje
                if(layer->isMaskActive()) {
                    if(layer->getMask() != NULL)  {
                        layer->applyLayerMask(painter);
                    }
                }
                // vykresli vrstvu do projektu
                layer->paintEvent(painter);
                // obnoveni
                painter.restore();
            }
        }
        break;
    case MASK_EDIT:
        // vykreli nahled jen aktualne vybrane masky (mode editace masek)
        if(this->selected_layer != NULL) {
            QBitmap *mask = this->selected_layer->getMask();
            if(mask != NULL) {
                // maska (nutne nastavit barvu pred volanim teto paint funkce, externe) !!!
                painter.drawPixmap(0, 0, *mask);

                // vykresli vrstvu do projektu
                painter.setOpacity(0.5);
                this->selected_layer->paintEvent(painter);
            } else {
                painter.setPen(QPen(Qt::red, 5));
                painter.drawLine(3, 3, this->size.width() - 3, this->size.height() - 3);
                painter.drawLine(3, this->size.height() - 3, this->size.width() - 3, 3);
            }
        }
        break;
    }
}

void WriteProjectToFile(const Project *project)
{
    if(project == NULL) return;

    QFile file(project->getPath());
    file.open(QIODevice::WriteOnly);
    QDataStream out(&file);

    // PROJECT #############################
    out << project->getName();
    out << project->getPath();
    out << project->getSize();
    out << project->getMode();

    // LAYERS ##############################
    if(project->getLayers() != NULL) {
        for(Layer *layer : *project->getLayers()) {
            layer->serialize(out);
        }
    }

    file.close();
}

Project *ReadProjectFromFile(const QString &projectPath)
{
    QFile file(projectPath);
    file.open(QIODevice::ReadOnly);
    QDataStream in(&file);

    // PROJECT #############################
    QString name;
    QString path;
    QSize size;
    ProjectEditMode_t mode;
    in >> name;
    in >> path;
    in >> size;
    in >> mode;
    Project *project = new Project(NULL, name, path, size);

    // LAYERS ##############################
    qint32 type;
    Layer *layer;
    bool end = false;
    while(!in.atEnd() && !end) {
        layer = NULL;
        // precte typ vrstvy a podle typy urci vrstvu
        in >> type;
        switch(type) {
        case BITMAP_LAYER_TYPE:
            layer = new BitmapLayer(project);
            break;
        case TEXT_LAYER_TYPE:
            layer = new TextLayer(project);
            break;
        case IMAGE_LAYER_TYPE:
            layer = new ImageLayer(project);
            break;
        default:
            end = true;
            break;
        }
        // deserializuje konkretni vrstvu a prida ji do projektu
        if(layer != NULL) {
            layer->deserialize(in);
            project->addLayerAtTop(layer);
            project->setSelectedLayer(layer);
        }
    }

    file.close();

    return project;
}
