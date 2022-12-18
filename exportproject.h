#ifndef EXPORTPROJECT_H
#define EXPORTPROJECT_H

#include <QWidget>

#include "base/app_context.h"

namespace Ui {
class ExportProject;
}

class ExportProject : public QWidget
{
    Q_OBJECT

public:
    explicit ExportProject(AppContext *context, QWidget *parent = nullptr);
    ~ExportProject();

protected:
    void paintPreview();

private slots:
    void on_pushButton_export_clicked();

    void on_pushButton_path_clicked();

private:
    Ui::ExportProject *ui;

    AppContext *context;

    float scale;
};

#endif // EXPORTPROJECT_H
