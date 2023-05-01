#ifndef IMPORTIMAGE_H
#define IMPORTIMAGE_H

#include <QWidget>

#include "base/app_context.h"


namespace Ui {
class ImportImage;
}

/**
 * @brief The ImportImage class
 */
class ImportImage : public QWidget
{
    Q_OBJECT
public:
    explicit ImportImage(AppContext *context, QWidget *parent = nullptr);
    ~ImportImage();

private slots:
    /**
     * @brief on_pushButton_import_clicked
     */
    void on_pushButton_import_clicked();

    /**
     * @brief on_pushButton_path_clicked
     */
    void on_pushButton_path_clicked();

private:
    Ui::ImportImage *ui;

    // kontext aplikace
    AppContext *context;
};

#endif // IMPORTIMAGE_H
