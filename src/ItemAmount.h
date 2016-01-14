//
//   File name      : ItemAmount.h
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-14 10:29:33
//

#ifndef _ITEMAMOUNT_H_
#define _ITEMAMOUNT_H_ 1

// Inherited header
// ...

// C system headers
// ...

// C++ system headers
#include <list>

// Headers from other projects
// ...

// Headers of current project
#include "ItemNamePair.h"

class ItemAmount
{
    const ItemNamePair kNamePair_;
    int count_;
public:
    ItemAmount(const ItemNamePair &kNamePair, int count)
    :   kNamePair_(kNamePair),
        count_(count){}
    ~ItemAmount(){}

    const ItemNamePair& name_pair() const { return kNamePair_; }
    int count() const { return count_; }
protected:
private:
};

typedef std::list<ItemAmount> ItemAmountList;

#endif
