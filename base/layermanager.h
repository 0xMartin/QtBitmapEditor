#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H


#include <QWidget>
#include <QListView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QLabel>
#include <QCheckBox>
#include <QSpacerItem>
#include <QSpinBox>
#include <QComboBox>
#include <QMenu>

#include "module.h"
#include "project.h"

/**
 * @brief Widget pro vrstvu. Urcen pro vkladani do list widgetu.
 */
class LayerWidget : public QWidget {
    Q_OBJECT
public:
    /**
     * @brief Vytvori widget vrstvy
     * @param layer - Vrstva
     * @param height - Vyska widgetu
     */
    explicit LayerWidget(Layer *layer, size_t height);
    ~LayerWidget();

    /**
     * @brief Navrati vrstvu ktere widget nalezi
     * @return Layer
     */
    Layer *getLayer() const;

    /**
     * @brief V tomto widgetu prekresli nahled vrstvy
     */
    void repaintLayer();

    /**
     * @brief V tomto widgetu prekresli nahled masky
     */
    void repaintMask();

    /**
     * @brief Zmeni jmeno widgetu
     * @param name - Jmeno
     */
    void setName(const QString &name);

protected:
    size_t height; /** Vyska obrazku nahledu vrstvy */
    Layer * layer; /** Vrstva ktere tento widget patri*/

    // UI
    QHBoxLayout *hBoxLayout;
    QLabel *image;
    QLabel *mask;
    QLabel *label;
    QCheckBox *checkBox_visible;

private slots:
    /**
     * @brief Zmena viditelnosti vrstvy
     * @param visible - Viditelnost
     */
    void on_checkBox_visible_toggle(bool visible);

};


/**
 * @brief Komponenta pro spravu vrstev projektu
 */
class LayerManager : public QWidget, public Module
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

    /**
     * @brief updateLayerList
     */
    void updateLayerList();

    /**
     * @brief Vybere novy pracovni mod projektu
     * @param mode - Pracovni mod
     */
    void selectMode(ProjectEditMode_t mode);

protected:
    // aktualni projekt
    Project *project;


    // layout
    QVBoxLayout *mainLayout;
    // project controll
    QWidget *projectControl;
    QHBoxLayout *projectControllLayout;
    // layer controll
    QWidget *layerControl;
    QHBoxLayout *layerControllLayout;
    // list controll
    QWidget *listControl;
    QHBoxLayout *listControllLayout;


    // hlavni komponenty ovladaci manazeru vrstev
    QLabel *header;
    // project controll
    QComboBox *comboBox_editMode;
    // layer controll
    QCheckBox *checkBox_antialiasing; /** Ovladani antialiasingu vybrane vrstvy */
    QSpinBox *spinbox_opacity; /** Ovladani opacity vybrane vrstvy */
    QComboBox *comboBox_blend; /** Ovladani blend modu vybrane vrstvy */
    //list
    QListWidget *listWidget; /** List se vsema vrstvama projektu */
    // list controll
    QPushButton *button_addLayer; /** Tlacitko pro pridani nove vrstvy */
    QPushButton *button_removeLayer; /** Tlacikto pro odebrani vybrane vrstvy */
    QPushButton *button_up; /** Tlacitko pro presunuti vrstvy nahoru */
    QPushButton *button_down; /** Tlacikto pro presunuti vrstvy dolu */
    QPushButton *button_mask; /** Tlacikto pro presunuti/odebrani masky vrstvy */


    virtual void changeEvent(QEvent * event) override;

    /**
     * @brief Update nabindovani prvku pro ovladani aktualne vybrane vrstvy v projektu
     */
    void updateLayerControllBinding();

private slots:
    /**
     * @brief Prekresleni vrstvy (vyvolano signale z projektu)
     * @param layer - Vrstva jejiz nahled se ma prekreslit
     */
    void on_project_repaintSignal(Layer *layer);

    /**
     * @brief Pozmenen list vrstev v projektu (vyvolano signale z projektu)
     */
    void on_project_layerListChanged();

    /*****************************************************************************************/
    // LIST CONTROLL
    /*****************************************************************************************/

    /**
     * @brief Pridani nove vrstvy
     */
    void on_button_addLayer_clicked();

    /**
     * @brief Odebrani aktualne vybrane vrstvy
     */
    void on_button_removeLayer_clicked();

    /**
     * @brief Presunuti aktualne vybrane vrstvy o jedno nahoru
     */
    void on_button_up_clicked();

    /**
     * @brief Presunuti aktualne vybrane vrstvy o jedno dolu
     */
    void on_button_down_clicked();

    /**
     * @brief Vytvoreni/Odstraneni masky u aktualne vybrane vrstvy
     */
    void on_button_mask_clicked();

    /**
     * @brief Spojeni dvou vrstev
     */
    void on_layer_merge_down();

    /**
     * @brief Duplikovani vybrane vrstev
     */
    void on_layer_duplicate();

    /**
     * @brief Rasterizuje vybranou vrstvu
     */
    void on_layer_rasterize();

    /**
     * @brief Aktivuje/Deaktivuje masku aktualne vybrane vrstvy
     */
    void on_mask_active_deactivate();

    /**
     * @brief Prejmenovani vybrane vrstvy
     */
    void on_layer_rename();

    /**
     * @brief V projektu nastavi aktualne vybranou vrstvu z listu
     */
    void on_listWidget_itemSelectionChanged();

    /**
     * @brief Double click event list -> tato udalost otevre dialog pro prejmenovani vrstvy
     * @param item - QListWidgetItem
     */
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    /**
     * @brief Zobrazi kontextove menu pro vybranou vrstvu
     * @param pos - Pozice
     */
    void showContextMenu(const QPoint &pos);

    /*****************************************************************************************/
    // LAYER CONTROLL
    /*****************************************************************************************/

    /**
     * @brief Zmena antialiasingu u vybrane vrstvy
     * @param value - Povolen / Nepovolen
     */
    void on_layer_antialiasing_toggled(bool value);

    /**
     * @brief Zmena blend modu u vybrane vrstvy
     * @param index - Index v listu
     */
    void on_layer_blend_changed(int index);

    /**
     * @brief Zmena opacity u vybrane vrstvy
     * @param value - Hodnota opacity
     */
    void on_layer_opacity_changed(int value);

    /*****************************************************************************************/
    // LAYER CONTROLL
    /*****************************************************************************************/

    /**
     * @brief Zmena editacniho modu projektu
     * @param index - Index v listu
     */
    void on_project_edit_mode_changed(int index);

};

#endif // LAYERMANAGER_H
