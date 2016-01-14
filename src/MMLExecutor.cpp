//
//   File name      : MMLExecutor.cpp
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-12 13:38:40
//

// Implemented header
#include "MMLExecutor.h"

// C system headers
// ...

// C++ system headers
// ...

// Headers from other projects
// ...

// Headers of current project
#include "DataHolderFileOperator.h"
#include "exception/MMLScriptFileFormatErrorException.h"
#include "MMLCommand.h"
#include "exception/UnknownMMLTypeException.h"
#include "exception/MMLNoSuchArgumentException.h"
#include "GlobalDataType.h"
#include "DatabaseOperator.h"
#include "exception/NoSuchItemNameException.h"
#include "IFOperator/NumberOperator.h"

MMLProcessorMap MMLExecutor::s_processors_;

void MMLExecutor::ExecuteSingleCommand(const MMLCommand &kCommand)
{
    MMLProcessorMap::const_iterator ite = s_processors_.find(kCommand.name());
    if (s_processors_.end() == ite)
    {
        throw UnknownMMLTypeException(kCommand.name());
    }
    ite->second(db_, kCommand.arguments());
    return;
}

void MMLExecutor::RegisterMMLProcessors()
{
    s_processors_[std::string("ADD-ITEM-INFO")] = AddItemInfo;
}

void MMLExecutor::AddItemInfo(DatabaseOperator &db, const MMLArgumentSet &kArguments)
{
    const ItemNamePair kNamePair = kArguments.GetItemNamePair(std::string("NAME"));
    const int kIDInGame = kArguments.GetInteger(std::string("ID_IN_GAME"));
    try
    {
        ItemID item_id = db.QueryItemID(kNamePair.name(), kNamePair.type_name());
        db.UpdateItemInfo(item_id, ITEMINFO_PTR(new ItemInfo(kNamePair, kIDInGame)));
    }
    catch (NoSuchItemNameException &)
    {
        db.AddItemInfo(ITEMINFO_PTR(new ItemInfo(kNamePair, kIDInGame)));
    }
    return;
}
