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

void MMLExecutor::ExecuteSingleCommand(const MMLCOMMAND_PTR &kCommand)
{
    MMLProcessorMap::const_iterator ite = s_processors_.find(kCommand->type());
    if (s_processors_.end() == ite)
    {
        throw UnknownMMLTypeException(kCommand->type());
    }
    ite->second(db_, kCommand->arguments());
    return;
}

void MMLExecutor::RegisterMMLProcessors()
{
    s_processors_[std::string("ADD-ITEM-INFO")] = AddItemInfo;
}

void MMLExecutor::AddItemInfo(DatabaseOperator &db, const MMLArguments &kArguments)
{
    const std::string kTypeName = GetMMLArgumentValue(kArguments, std::string("type-name"));
    const std::string kItemName = GetMMLArgumentValue(kArguments, std::string("item-name"));
    const ItemIDInGame kIDInGame = NumberOperator::AtoI(GetMMLArgumentValue(kArguments, std::string("id-in-game")));
    try
    {
        ItemID item_id = db.QueryItemID(kTypeName, kItemName);
        db.UpdateItemInfo(item_id, ITEMINFO_PTR(new ItemInfo(kItemName, kTypeName, kIDInGame)));
    }
    catch (NoSuchItemNameException &)
    {
        db.AddItemInfo(ITEMINFO_PTR(new ItemInfo(kItemName, kTypeName, kIDInGame)));
    }
    return;
}

//////////////////////////////////////////////////////////////////////////
const std::string& MMLExecutor::GetMMLArgumentValue(const MMLArguments &kArguments, const std::string &kName)
{
    MMLArguments::const_iterator ite = kArguments.find(kName);
    if (kArguments.end() == ite)
    {
        throw MMLNoSuchArgumentException(kName);
    }
    return ite->second;
}
