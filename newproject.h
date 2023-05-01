#ifndef NEWPROJECT_H
#define NEWPROJECT_H

#include <QWidget>

#include "base/app_context.h"


namespace Ui {
class NewProject;
}

/**
 * @brief The NewProject class
 */
class NewProject : public QWidget
{
    Q_OBJECT

public:
    explicit NewProject(AppContext *context, QWidget *parent = nullptr);
    ~NewProject();

private slots:
    /**
     * @brief on_pushButton_clicked
     */
    void on_pushButton_clicked();

    /**
     * @brief on_pushButton_path_clicked
     */
    void on_pushButton_path_clicked();

private:
    Ui::NewProject *ui;

    // kontext aplikace
    AppContext *context;

signals:
    void projectCreated();
};

#endif // NEWPROJECT_H
