#ifndef TOOL_H
#define TOOL_H

#include <QWidget>

class Tool
{
public:
    Tool();

    /**
     * @brief Navrati list controll elementu
     * @return QList<QWidget*>
     */
    const QList<QWidget*> &getControlls() const;
    /**
     * @brief addControll
     * @param controll - Novy controll widged (control == QWidged, ktery manimuluje s parametry zvoleneho nastroje)
     * @return True -> uspech
     */
    bool addControll(QWidget *controll);

protected:
    QList<QWidget*> controlls;
};

#endif // TOOL_H
