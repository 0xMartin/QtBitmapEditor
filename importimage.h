#ifndef IMPORTIMAGE_H
#define IMPORTIMAGE_H

#include <QWidget>

#include "base/app_context.h"


namespace Ui {
class ImportImage;
}

class ImportImage : public QWidget
{
    Q_OBJECT
public:
    explicit ImportImage(QWidget *parent = nullptr);
    ~ImportImage();

    void setContext(AppContext *context);

private slots:
    void on_pushButton_import_clicked();

    void on_pushButton_path_clicked();

private:
    Ui::ImportImage *ui;

    AppContext *context;
};

#endif // IMPORTIMAGE_H
