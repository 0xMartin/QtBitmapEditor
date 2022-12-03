#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include <QWidget>
#include <QListView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QAbstractListModel>
#include <QList>

#include "../objects/project.h"

/**
 * @brief Listmodel s vrstvama projektu
 */
class LayerListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    explicit LayerListModel(QObject *parent = 0);
    QVariant data(const QModelIndex &index, int role) const override;
    int rowCount(const QModelIndex &parent) const override;
    void setList(Layers_t *list);
    Layer *getLayerAt(size_t index);
private:
    Layers_t *list;
};

/**
 * @brief Komponenta pro spravu vrstev projektu
 */
class LayerManager : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Vytvori LayerManager
     * @param parent - QWidget
     */
    explicit LayerManager(QWidget *parent = nullptr);

    ~LayerManager();

    /**
     * @brief Nastavi projekt se ktery se bude pracovat
     * @param project - Pointer na projekt
     */
    void setProject(Project *project);

    /**
     * @brief Navrati pointer na projekt
     * @return Pointer na projekt
     */
    Project *getProject() const;

protected:
    // aktualni projekt
    Project *project;

    // list view model
    LayerListModel *listModel;

    // layout
    QHBoxLayout *buttonsLayout;
    QVBoxLayout *mainLayout;
    QWidget * buttons;

    // hlavni komponenty manazeru vrstev
    QListView * listView; /** List se vsema vrstvama projektu */
    QPushButton * button_addLayer; /** Tlacitko pro pridani nove vrstvy */
    QPushButton * button_removeLayer; /** Tlacikto pro odebrani vybrane vrstvy */

private slots:
    /**
     * @brief Pridani nove vrstvy
     */
    void on_button_addLayer_clicked();

    /**
     * @brief Odebrani aktualne vybrane vrstvy
     */
    void on_button_removeLayer_clicked();

    /**
     * @brief V projektu nastavi aktualne vybranou vrstvu z listu
     */
    void on_listView_clicked(const QModelIndex &index);
};

#endif // LAYERMANAGER_H
