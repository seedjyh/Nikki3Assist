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
#include "exception/DuplicateItemTypeException.h"
#include "exception/NoWayToAcquireItemException.h"

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
    s_processors_[std::string("ADD-ITEM-TYPE")] = AddItemType;
    s_processors_[std::string("ADD-ITEM-INFO")] = AddItemInfo;
    s_processors_[std::string("SET-ITEM-PRICE")] = SetItemPrice;
    s_processors_[std::string("SET-ITEM-STOCK")] = SetItemStock;
    s_processors_[std::string("SET-CREATING-RULE")] = SetCreatingRule;
    s_processors_[std::string("SET-DYEING-RULE")] = SetDyeingRule;
    s_processors_[std::string("ADD-TASK-INFO")] = AddTaskInfo;
    s_processors_[std::string("SHOW-ITEM-ACQUISITION-MEAN")] = ShowItemAcquisitionMean;
}

void MMLExecutor::AddItemType(DatabaseOperator &db, const MMLArgumentSet &kArguments)
{
    db.AddItemType(kArguments.GetString(std::string("TYPE")));
    return;
}


void MMLExecutor::AddItemInfo(DatabaseOperator &db, const MMLArgumentSet &kArguments)
{
    db.AddItemInfo(
        ITEMINFO_PTR(
            new ItemInfo(
                kArguments.GetItemNamePair(std::string("NAME")),
                kArguments.GetInteger(std::string("ID_IN_GAME"))
            )
        )
    );
    return;
}

void MMLExecutor::SetItemPrice(DatabaseOperator &db, const MMLArgumentSet &kArguments)
{
    db.SetItemPrice(
        kArguments.GetItemNamePair(std::string("ITEM")),
        kArguments.GetItemAmountList(std::string("PRICE"))
    );
    return;
}

void MMLExecutor::SetItemStock(DatabaseOperator &db, const MMLArgumentSet &kArguments)
{
    db.SetItemStock(
        kArguments.GetItemNamePair(std::string("ITEM")),
        kArguments.GetInteger(std::string("COUNT"))
    );
    return;
}

void MMLExecutor::SetCreatingRule(DatabaseOperator &db, const MMLArgumentSet &kArguments)
{
    db.SetItemCreatingRule(
        kArguments.GetItemNamePair(std::string("PRODUCT")),
        kArguments.GetItemAmountList(std::string("RAW_MATERIALS"))
    );
    return;
}

void MMLExecutor::SetDyeingRule(DatabaseOperator &db, const MMLArgumentSet &kArguments)
{
    db.SetItemDyeingRule(
        kArguments.GetItemNamePair(std::string("PRODUCT")),
        kArguments.GetItemAmountList(std::string("RAW_MATERIALS"))
    );
    return;
}

void MMLExecutor::AddTaskInfo(DatabaseOperator &db, const MMLArgumentSet &kArguments)
{
    const TaskName kTaskName(
        kArguments.GetString(std::string("TYPE")),
        kArguments.GetString(std::string("CHAPTER")),
        kArguments.GetString(std::string("STAGE"))
    );
    db.AddTask(kTaskName);
    db.AddTaskReward(kTaskName, kArguments.GetItemAmountList(std::string("REWARDS")));
    return;
}

void MMLExecutor::ShowItemAcquisitionMean(DatabaseOperator &db, const MMLArgumentSet &kArguments)
{
    try
    {
        ITEMACQUISITION_PTR result = db.QueryItemAcquisitionMean(
            kArguments.GetItemAmountList(std::string("TARGET"))
        );
        std::cout << *result << std::endl;
    }
    catch (NoWayToAcquireItemException &e)
    {
        std::cout << e.message() << std::endl;
    }
    return;
}
