//
//   File name      : MMLArgumentSet.h
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-14 10:46:10
//

#ifndef _MMLARGUMENTSET_H_
#define _MMLARGUMENTSET_H_ 1

// Inherited header
// ...

// C system headers
// ...

// C++ system headers
#include <string>
#include <set>
#include <map>

// Headers from other projects
// ...

// Headers of current project
#include "ItemNamePair.h"
#include "ItemAmount.h"

typedef std::string ArgumentName;
typedef std::set<ArgumentName> ArgumentNameSet;
typedef std::map<std::string, int> IntegerArgumentMap;
typedef std::map<std::string, std::string> StringArgumentMap;
typedef std::map<std::string, ItemNamePair> ItemNamePairArgumentMap;
typedef std::map<std::string, ItemAmountList> ItemAmountListArgumentMap;

class MMLArgumentSet
{
    ArgumentNameSet all_argument_name_set_;
    IntegerArgumentMap integer_arguments_;
    StringArgumentMap string_arguments_;
    ItemNamePairArgumentMap item_name_pair_arguments_;
    ItemAmountListArgumentMap item_amount_list_arugments_;

public:
    MMLArgumentSet(){}
    ~MMLArgumentSet(){}

    void AppendInteger(const ArgumentName &kName, int value);
    void AppendString(const ArgumentName &kName, const std::string &kValue);
    void AppendItemNamePair(const ArgumentName &kName, const ItemNamePair &kValue);
    void AppendItemAmountList(const ArgumentName &kName, const ItemAmountList &kValue);

    int GetInteger(const ArgumentName &kName) const;
    const std::string& GetString(const ArgumentName &kName) const;
    const ItemNamePair& GetItemNamePair(const ArgumentName &kName) const;
    const ItemAmountList& GetItemAmountList(const ArgumentName &kName) const;
protected:
private:
    void AssureNoDuplicateArgument(const ArgumentName &kName) const;
};

#endif
