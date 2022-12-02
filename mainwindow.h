#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QScrollArea>
#include <QListWidget>
#include <QWidget>
#include <QSplitter>

// hlavni komponenty
#include "component/workspace.h"
#include "component/toolcontroller.h"
#include "component/layermanager.h"

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
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    // hlavni komponenty
    Workspace * workspace;
    ToolController * toolController;
    LayerManager * layerManager;

private slots:
    void on_actionNew_project_triggered();

    void on_actionOpen_project_triggered();

    void on_actionImport_image_triggered();

    void on_actionExport_image_triggered();

    void on_actionExit_triggered();

    void on_actionPrint_triggered();

    void on_actionAbout_triggered();

    void on_actionPen_triggered();

    void on_actionBrush_triggered();

    void on_actionFill_triggered();

    void on_actionText_triggered();

    void on_actionCircle_triggered();

    void on_actionPolygon_triggered();

    void on_actionEraser_triggered();

    void on_actionAdd_layer_triggered();

    void on_actionRemove_layer_triggered();

    void on_actionRectangle_triggered();

private:
    // UI
    Ui::MainWindow *ui;
    QSplitter* splitter_horizontal;
    QSplitter* splitter_vertical;
    QScrollArea *scrollArea_workspace;

};
#endif // MAINWINDOW_H
