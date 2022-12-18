#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QListWidget>
#include <QWidget>
#include <QSplitter>
#include <QLabel>


#include "base/app_context.h"
#include "utility/colorpicker.h"
#include "newproject.h"
#include "openproject.h"
#include "importimage.h"


/**
 * Hlavni okno aplikace
 */
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(AppContext *context, QWidget *parent = nullptr);
    ~MainWindow();

protected:
    // kontext aplikace
    AppContext *context;

    // utility
    ColorPicker *colorPicker;

    // windows
    NewProject *window_newProject;
    OpenProject *window_openProject;
    ImportImage *window_importImage;

private slots:
    /**
     * @brief Otevre okno pro vytvoreni noveho projektu
     */
    void on_actionNew_project_triggered();

    /**
     * @brief Otevre file dialog pro otevreni existujiciho projektu
     */
    void on_actionOpen_project_triggered();

    /**
     * @brief Ulozi projekt na disk
     */
    void on_actionSave_project_triggered();

    /**
     * @brief Otevre file dialog pro importovani obrazku do projektu
     */
    void on_actionImport_image_triggered();

    /**
     * @brief Otevre okno pro exportovani obrazku
     */
    void on_actionExport_image_triggered();

    /**
     * @brief Ukoncni aplikaci (pred ukonceni zobrazi dialog pro ulozeni projektu)
     */
    void on_actionExit_triggered();

    /**
     * @brief Otevre okno s nahledem tisku a moznosti tisku tohoto obrazku
     */
    void on_actionPrint_triggered();

    /**
     * @brief Otevre okno s inforamacemi o teto aplikaci
     */
    void on_actionAbout_triggered();

    /**
     * @brief Vybera nastro "eye dropper"
     */
    void on_actionEye_Dropper_triggered();

    /**
     * @brief Vybere nastroj "pen"
     */
    void on_actionPen_triggered();

    /**
     * @brief Vybere nastroj "brush"
     */
    void on_actionBrush_triggered();

    /**
     * @brief Vybere nastroj "fill"
     */
    void on_actionFill_triggered();

    /**
     * @brief Vybere nastroj "text"
     */
    void on_actionText_triggered();

    /**
     * @brief Vybere nastroj "Eraser"
     */
    void on_actionEraser_triggered();

    /**
     * @brief Prida novou vrstu
     */
    void on_actionAdd_layer_triggered();

    /**
     * @brief Odstrani aktualne oznacenou vrstvu
     */
    void on_actionRemove_layer_triggered();

    void on_actionZoom_in_triggered();

    void on_actionZoom_out_triggered();

    void on_actionSwitch_Image_Mask_triggered();

public slots:
    /**
     * @brief Update staus baru
     */
    void updateStatusBar();

private:
    // UI
    Ui::MainWindow *ui;
    QSplitter* splitter_horizontal;
    QSplitter* splitter_vertical;
    QLabel *statusLabel;

    void highlightToolbar(QAction *action);

};
#endif // MAINWINDOW_H
