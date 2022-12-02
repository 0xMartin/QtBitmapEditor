#ifndef NEWPROJECT_H
#define NEWPROJECT_H

#include <QWidget>

namespace Ui {
class NewProject;
}

class NewProject : public QWidget
{
    Q_OBJECT

public:
    explicit NewProject(QWidget *parent = nullptr);
    ~NewProject();

private:
    Ui::NewProject *ui;
};

#endif // NEWPROJECT_H
