#include "tool.h"

Tool::Tool()
{

}

const QList<QWidget*> &Tool::getControlls() const
{
    return this->controlls;
}

bool Tool::addControll(QWidget *controll)
{
    if(controll == NULL) return false;
    this->controlls.push_back(controll);

    return true;
}
