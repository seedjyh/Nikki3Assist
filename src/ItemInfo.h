//
//   File name      : ItemInfo.h
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-11 13:41:24
//

#ifndef _ITEMINFO_H_
#define _ITEMINFO_H_ 1

// Inherite header
// ...

// C system headers
// ...

// C++ system headers
#include <boost/shared_ptr.hpp>

// Headers from other projects
// ...

// Headers of current project
#include "GlobalDataType.h"
#include "ItemNamePair.h"

// This class would not contain ItemUID.
class ItemInfo
{
    const ItemNamePair kNamePair_;
    const ItemIDInGame kIDInGame_;
public:
    ItemInfo(
        const ItemNamePair &kNamePair,
        const ItemIDInGame &kIDInGame
    ) : kNamePair_(kNamePair),
        kIDInGame_(kIDInGame){}
    ~ItemInfo(){}

    const ItemNamePair& name_pair() const { return kNamePair_; }
    const ItemIDInGame id_in_game() const { return kIDInGame_; }
    void Print(){}
protected:
private:
};

typedef boost::shared_ptr<ItemInfo> ITEMINFO_PTR;

#endif
