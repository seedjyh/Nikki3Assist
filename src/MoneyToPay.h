//
//   File name      : MoneyToPay.h
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-18 09:41:46
//

#ifndef _MONEYTOPAY_H_
#define _MONEYTOPAY_H_ 1

// Inherited header
// ...

// C system headers
// ...

// C++ system headers
#include <list>
#include <iostream>

// Headers from other projects
// ...

// Headers of current project
#include "ItemAmount.h"

class MoneyToPay
{
    ItemAmount money_;
    ItemAmountList targets_;
public:
    MoneyToPay(
        const ItemAmount &kMoney,
        const ItemAmountList &kTargets
    ) : money_(kMoney),
        targets_(kTargets){}
    ~MoneyToPay(){}

    friend std::ostream& operator<<(std::ostream &output, const MoneyToPay &kThis)
    {
        output << kThis.money_ << " to buy: " << std::endl;
        for (ItemAmountList::const_iterator ite = kThis.targets_.begin(); ite != kThis.targets_.end(); ++ite)
        {
            output << *ite << std::endl;
        }
        return output;
    }
protected:
private:
};

typedef std::list<MoneyToPay> MoneyToPayList;

#endif
