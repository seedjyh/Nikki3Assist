//
//   File name      : DatabaseOperator.h
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-11 13:26:53
//

#ifndef _DATABASEOPERATOR_H_
#define _DATABASEOPERATOR_H_ 1

// Inherite header
// ...

// C system headers
// ...

// C++ system headers
#include <string>

// Headers from other projects
// ...

// Headers of current project
#include "GlobalDataType.h"
#include "ItemInfo.h"
#include "ItemAcquisition.h"
#include "IFDataType/DataHolder.h"
#include "IFDataType/Tstring.h"
#include "IFSQLiteAdaptor/SQLiteHandle.h"

// Generate SQLs and execute them.
class DatabaseOperator
{
    SQLITEHANDLE_PTR db_handler_;
public:
    DatabaseOperator(const Tstring &kFilePath);
    ~DatabaseOperator(){}

    // About ItemInfo
    void AddItemType(const std::string &kTypeName);
    void AddItemInfo(const ITEMINFO_PTR &kItemInfo); // ItemType must exists. Generate new item id.
    ItemID QueryItemID(const std::string &kItemName, const std::string &kTypeName);
    void DeleteItemInfo(const ItemID &kID); // All records in all tables referring to this ID would be deleted.
    void UpdateItemInfo(const ItemID &kID, const ITEMINFO_PTR &kItemInfo);
    ITEMINFO_PTR QueryItemInfo(const ItemID &kID);

    // About Inventory
    void SetItemStock(const ItemID &kID, int stock); // kID should exist in ItemInfo table.

    // Rules can be covered, but can't be deleted directory. Rules would be deleted only if the item was deleted from ItemInfo table.
    void SetItemCreatingRule(const ItemID &kID, const Materials &kRawMaterials);
    void SetItemDyeingRule(const ItemID &kID, const Materials &kRawMaterials);

    // About store.
    void SetItemPrice(const ItemID &kID, const Materials &kPrice); // kPrice should only contain "coin" type item and should exist in ItemInfo table.
    ITEMACQUISITION_PTR QueryItemAcquisitionMean(const ItemID &kID){ return ITEMACQUISITION_PTR(new ItemAcquisition()); }

    // About tasks.
    void AddTaskType(const std::string &kType);
    void AddTaskChapter(const std::string &kType, const std::string &kChapter); // Type must exists.
    void AddTaskStage(const std::string &kType, const std::string &kChapter, const std::string &kStage); // Type and chapter must exist. Generate new task id.
    TaskID QueryTaskID(const std::string &kType, const std::string &kChapter, const std::string &kStage);
    void AddTaskReward(TaskID task_id, ItemID reward_uid);
protected:
private:
    void CreateEmptyTables();
    void CreateTriggers();
    void DeleteAllTables();
    bool CheckTablesIntegrity();
    bool CheckDataConsistency(){ return true; }
    ItemTypeID QueryItemTypeID(const std::string &kItemTypeName); // throw DatabaseInconsistencyException if not found.
    const std::string QueryItemTypeName(const ItemTypeID &kID);
private:
    // Check sql execute result. If it failed, throw ExecuteSQLFailedException.
    static void AssertSQLReturnCode(const std::string &kSQL, const SQLEXECUTERESULT_PTR &kSQLResult);
};

#endif
