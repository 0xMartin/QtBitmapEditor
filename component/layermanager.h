#ifndef LAYERMANAGER_H
#define LAYERMANAGER_H

#include <QWidget>
#include <QListWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>


/**
 * @brief The LayerManager class
 */
class LayerManager : public QWidget
{
    Q_OBJECT

public:
    explicit LayerManager(QWidget *parent = nullptr);

    ~LayerManager();

protected:
    // layout
    QHBoxLayout *buttonsLayout;
    QVBoxLayout *mainLayout;
    QWidget * buttons;

    // hlavni komponenty manazeru vrstev
    QListWidget * listWidget; /** List se vsema vrstvama projektu */
    QPushButton * button_addLayer; /** Tlacitko pro pridani nove vrstvy */
    QPushButton * button_removeLayer; /** Tlacikto pro odebrani vybrane vrstvy */
};

#endif // LAYERMANAGER_H
