#ifndef NEWPROJECT_H
#define NEWPROJECT_H

#include <QWidget>

#include "base/app_context.h"

namespace Ui {
class NewProject;
}

class NewProject : public QWidget
{
    Q_OBJECT

public:
    explicit NewProject(QWidget *parent = nullptr);
    ~NewProject();

    void setContext(AppContext *context);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_path_clicked();

private:
    Ui::NewProject *ui;

    AppContext *context;
};

#endif // NEWPROJECT_H
