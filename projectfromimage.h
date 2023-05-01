#ifndef PROJECTFROMIMAGE_H
#define PROJECTFROMIMAGE_H

#include <QWidget>

#include "base/app_context.h"


namespace Ui {
class ProjectFromImage;
}

/**
 * @brief The NewProject class
 */
class ProjectFromImage : public QWidget
{
    Q_OBJECT

public:
    explicit ProjectFromImage(AppContext *context, QWidget *parent = nullptr);
    ~ProjectFromImage();

private slots:
    /**
     * @brief on_pushButton_clicked
     */
    void on_pushButton_clicked();

    /**
     * @brief on_pushButton_path_clicked
     */
    void on_pushButton_path_clicked();

    void on_pushButton_path_2_clicked();

private:
    Ui::ProjectFromImage *ui;

    // kontext aplikace
    AppContext *context;

signals:
    void projectCreated();
};

#endif // NEWPROJECT_H
