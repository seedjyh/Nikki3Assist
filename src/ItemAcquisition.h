//
//   File name      : ItemAcquisition.h
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-11 13:43:22
//

#ifndef _ITEMACQUISITION_H_
#define _ITEMACQUISITION_H_ 1

// Inherite header
// ...

// C system headers
// ...

// C++ system headers
#include <boost/shared_ptr.hpp>

// Headers from other projects
// ...

// Headers of current project
#include "TaskToDo.h"
#include "MoneyToPay.h"
#include "ItemAmount.h"

class ItemAcquisition
{
    TaskToDoList task_to_do_;
    MoneyToPayList money_to_pay_;
    ItemAmountList items_unknown_how_to_get_;
public:
    ItemAcquisition(){}
    ~ItemAcquisition(){}

    void AppendTaskToDo(const TaskToDo &kTaskToDo);
    void AppendMoneyToPay(const MoneyToPay &kMoneyToPay);
    void AppendItemsUnknownHowToAcquire(const ItemAmount &kItemAmount);

    friend std::ostream& operator<<(std::ostream &output, const ItemAcquisition &kThis);
protected:
private:
};

typedef boost::shared_ptr<ItemAcquisition> ITEMACQUISITION_PTR;

#endif
