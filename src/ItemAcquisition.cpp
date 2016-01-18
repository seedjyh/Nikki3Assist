//
//   File name      : ItemAcquisition.cpp
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-18 09:45:38
//

// Implemented header
#include "ItemAcquisition.h"

// C system headers
// ...

// C++ system headers
#include <iostream>

// Headers from other projects
// ...

// Headers of current project
// ...

void ItemAcquisition::AppendTaskToDo(const TaskToDo &kTaskToDo)
{
    task_to_do_.push_back(kTaskToDo);
    return;
}

void ItemAcquisition::AppendMoneyToPay(const MoneyToPay &kMoneyToPay)
{
    money_to_pay_.push_back(kMoneyToPay);
    return;
}

void ItemAcquisition::AppendItemsUnknownHowToAcquire(const ItemAmount &kItemAmount)
{
    items_unknown_how_to_get_.push_back(kItemAmount);
    return;
}

std::ostream& operator<<(std::ostream &output, const ItemAcquisition &kThis)
{
    std::cout << "ACQUISITION:" << std::endl;
    std::cout << "TASK TO DO:" << std::endl;
    for (TaskToDoList::const_iterator ite = kThis.task_to_do_.begin(); ite != kThis.task_to_do_.end(); ++ite)
    {
        std::cout << *ite;
    }
    std::cout << "MONEY TO PAY:" << std::endl;
    for (MoneyToPayList::const_iterator ite = kThis.money_to_pay_.begin(); ite != kThis.money_to_pay_.end(); ++ite)
    {
        std::cout << *ite;
    }
    std::cout << "UNKNOWN HOW TO ACQUIRE:" << std::endl;
    for (ItemAmountList::const_iterator ite = kThis.items_unknown_how_to_get_.begin(); ite != kThis.items_unknown_how_to_get_.end(); ++ite)
    {
        std::cout << *ite;
    }
    return output;
}
