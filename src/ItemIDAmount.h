//
//   File name      : ItemIDAmount.h
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-18 15:13:11
//

#ifndef _ITEMIDAMOUNT_H_
#define _ITEMIDAMOUNT_H_ 1

// Inherited header
// ...

// C system headers
// ...

// C++ system headers
// ...

// Headers from other projects
// ...

// Headers of current project
#include "GlobalDataType.h"

class ItemIDAmount
{
    ItemID id_;
    int count_;
public:
    ItemIDAmount(ItemID id, int count)
    :   id_(id),
        count_(count){}
    ~ItemIDAmount(){}

    ItemID id() const { return id_; }
    int count() const { return count_; }
protected:
private:
};

#endif
