#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include <QWidget>
#include <QListView>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>

#include "../objects/project.h"

/**
 * @brief The LayerManager class
 */
class LayerManager : public QWidget
{
    Q_OBJECT

public:
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
};

#endif // LAYERMANAGER_H
