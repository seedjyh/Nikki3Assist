//
//   File name      : MMLArgumentSet.cpp
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-14 10:54:18
//

// Implemented header
#include "MMLArgumentSet.h"

// C system headers
#include <assert.h>

// C++ system headers
// ...

// Headers from other projects
// ...

// Headers of current project
#include "exception/DuplicateArgumentNameException.h"
#include "exception/NoSuchArgumentException.h"

void MMLArgumentSet::AppendInteger(const ArgumentName &kName, int value)
{
    AssureNoDuplicateArgument(kName);
    integer_arguments_[kName] = value;
    return;
}

void MMLArgumentSet::AppendString(const ArgumentName &kName, const std::string &kValue)
{
    AssureNoDuplicateArgument(kName);
    string_arguments_[kName] = kValue;
    return;
}

void MMLArgumentSet::AppendItemNamePair(const ArgumentName &kName, const ItemNamePair &kValue)
{
    AssureNoDuplicateArgument(kName);
    item_name_pair_arguments_[kName] = kValue;
    return;
}

void MMLArgumentSet::AppendItemAmountList(const ArgumentName &kName, const ItemAmountList &kValue)
{
    AssureNoDuplicateArgument(kName);
    item_amount_list_arugments_[kName] = kValue;
}

int MMLArgumentSet::GetInteger(const ArgumentName &kName) const
{
    IntegerArgumentMap::const_iterator ite = integer_arguments_.find(kName);
    if (integer_arguments_.end() == ite)
    {
        throw NoSuchArgumentException(kName);
    }
    return ite->second;
}

const std::string& MMLArgumentSet::GetString(const ArgumentName &kName) const
{
    StringArgumentMap::const_iterator ite = string_arguments_.find(kName);
    if (string_arguments_.end() == ite)
    {
        throw NoSuchArgumentException(kName);
    }
    return ite->second;
}

const ItemNamePair& MMLArgumentSet::GetItemNamePair(const ArgumentName &kName) const
{
    ItemNamePairArgumentMap::const_iterator ite = item_name_pair_arguments_.find(kName);
    if (item_name_pair_arguments_.end() == ite)
    {
        throw NoSuchArgumentException(kName);
    }
    return ite->second;
}

const ItemAmountList& MMLArgumentSet::GetItemAmountList(const ArgumentName &kName) const
{
    ItemAmountListArgumentMap::const_iterator ite = item_amount_list_arugments_.find(kName);
    if (item_amount_list_arugments_.end() == ite)
    {
        throw NoSuchArgumentException(kName);
    }
    return ite->second;
}

//////////////////////////////////////////////////////////////////////////
void MMLArgumentSet::AssureNoDuplicateArgument(const ArgumentName &kName) const
{
    if (all_argument_name_set_.find(kName) != all_argument_name_set_.end())
    {
        throw DuplicateArgumentNameException(kName);
    }
    assert(integer_arguments_.end() == integer_arguments_.find(kName));
    assert(string_arguments_.end() == string_arguments_.find(kName));
    assert(item_name_pair_arguments_.end() == item_name_pair_arguments_.find(kName));
    assert(item_amount_list_arugments_.end() == item_amount_list_arugments_.find(kName));
    return;
}