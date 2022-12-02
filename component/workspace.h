#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <QWidget>

/**
 * @brief The Workspace class
 */
class Workspace : public QWidget
{
    Q_OBJECT

public:
    explicit Workspace(QWidget *parent = nullptr);

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

};

#endif // WORKSPACE_H
