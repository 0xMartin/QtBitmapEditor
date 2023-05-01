#ifndef PROJECTWIZARD_H
#define PROJECTWIZARD_H

#include <QWidget>
#include <QPushButton>

#include "base/app_context.h"
#include "newproject.h"
#include "openproject.h"
#include "projectfromimage.h"

namespace Ui {
class ProjectWizard;
}

/**
 * @brief The ProjectWizard class
 */
class ProjectWizard : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectWizard(AppContext *context, QWidget *parent = nullptr);
    ~ProjectWizard();

private slots:
    /**
     * @brief on_pushButtonCreate_clicked
     */
    void on_pushButtonCreate_clicked();

    /**
     * @brief on_pushButtonOpen_clicked
     */
    void on_pushButtonOpen_clicked();

    /**
     * @brief on_pushButtonFromImage_clicked
     */
    void on_pushButtonFromImage_clicked();

    /**
     * @brief Pokud je vytvoren projekt
     */
    void projectCreated();

private:
    Ui::ProjectWizard *ui;

    // kontext aplikace
    AppContext *context;

    // windows
    NewProject *window_newProject;
    OpenProject *window_openProject;
    ProjectFromImage *window_projectFromImage;

protected:
    void hightLightButton(QPushButton *button);

    void selectWidget(QWidget *widget);
};

#endif // PROJECTWIZARD_H
