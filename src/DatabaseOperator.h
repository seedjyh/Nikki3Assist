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
#include <set>

// Headers from other projects
// ...

// Headers of current project
#include "GlobalDataType.h"
#include "ItemInfo.h"
#include "ItemAcquisition.h"
#include "IFDataType/DataHolder.h"
#include "IFDataType/Tstring.h"
#include "IFSQLiteAdaptor/SQLiteHandle.h"
#include "ItemAmount.h"
#include "ItemIDAmount.h"

// Generate SQLs and execute them.
// Checking database inconsistency inside.
// "ID" stuff would not appear at the header file.
class DatabaseOperator
{
    SQLITEHANDLE_PTR db_handler_;
public:
    DatabaseOperator(const Tstring &kFilePath);
    ~DatabaseOperator(){}

    // About ItemInfo
    /*done*/void AddItemType(const std::string &kTypeName);
    /*done*/void AddItemInfo(const ITEMINFO_PTR &kItemInfo); // ItemType must exists. Generate new item id.

    // About store.
    /*done*/void SetItemPrice(const ItemNamePair &kItemNamePair, const ItemAmountList &kPrice); // kPrice should only contain "coin" type item and should exist in ItemInfo table.

    // About Inventory
    /*done*/void SetItemStock(const ItemNamePair &kItemNamePair, int count); // kID should exist in ItemInfo table.

    // Rules can be covered, but can't be deleted directory. Rules would be deleted only if the item was deleted from ItemInfo table.
    /*done*/void SetItemCreatingRule(const ItemNamePair &kProductItemNamePair, const ItemAmountList &kRawMaterials);
    /*done*/void SetItemDyeingRule(const ItemNamePair &kProductItemNamePair, const ItemAmountList &kRawMaterials);

    // About tasks.
    /*done*/void AddTask(const TaskName &kTaskName); // Generate new task id.
    /*done*/void AddTaskReward(const TaskName &kTaskName, const ItemAmountList &kRewards);

    ITEMACQUISITION_PTR QueryItemAcquisitionMean(const ItemAmountList &kTargets);
protected:
private:
    typedef std::set<TaskID> TaskRewards;
    void CreateEmptyTables();
    void CreateTriggers();
    void DeleteAllTables();
    bool CheckTablesIntegrity();
    bool CheckDataConsistency(){ return true; }
    /*done*/ItemTypeID QueryItemTypeID(const std::string &kItemTypeName); // throw DatabaseInconsistencyException if not found.
    /*done*/ItemID QueryItemID(const ItemNamePair &kItemNamePair);
    /*done*/bool CheckItemTypeExistence(const std::string &kItemTypeName);
    /*done*/void InsertItemType(const std::string &kItemTypeName);
    /*done*/bool CheckItemInfoExistence(const ItemNamePair &kItemNamePair);
    /*done*/void InsertItemInfo(ItemTypeID type_id, const ITEMINFO_PTR &kItemInfo);
    /*done*/int QueryItemStock(ItemID item_id);
    /*done*/bool CheckItemIDExistence(ItemID item_id);
    /*done*/void DeleteItemPrice(ItemID item_id);
    /*done*/void InsertItemPrice(const ItemID &kID, const ItemIDAmountMap &kPrice);
    /*done*/void DeleteItemStock(ItemID item_id);
    /*done*/void InsertItemStock(ItemID item_id, int count);
    /*done*/void DeleteItemCreatingRule(ItemID product_id);
    /*done*/void InsertItemCreatingRule(ItemID product_id, const ItemIDAmountMap &kMaterials);
    /*done*/void DeleteItemDyeingRule(ItemID product_id);
    /*done*/void InsertItemDyeingRule(ItemID product_id, const ItemIDAmountMap &kMaterials);
    /*done*/bool CheckTaskExistence(const TaskName &kTaskName);
    /*done*/void InsertTask(const TaskName &kTaskName);
    /*done*/TaskID QueryTaskID(const TaskName &kTaskName);
    /*done*/void DeleteTaskRewards(TaskID task_id);
    /*done*/void InsertTaskRewards(TaskID task_id, const TaskRewards &kRewards);
    /*done*/ItemIDAmountMap QueryAllItemInventory();
    void TryMakeItem(const ItemIDAmount &kTarget, ItemIDAmountMap &i_o_inventory, ItemIDAmountMap &i_o_required_extra_basic_items);
    ITEMACQUISITION_PTR QueryBasicItemAcquisitionMean(const ItemIDAmountMap &kTargets);
    TaskIDSet QueryTaskToGetTargetItem(ItemID target);
    ItemIDAmountMap QueryMoneyToBuyTargetItem(const ItemIDAmount &kTarget);
    ItemIDAmountMap QueryMaterialsForCreating(const ItemIDAmount &kTarget);
    ItemIDAmountMap QueryMaterialsForDyeing(const ItemIDAmount &kTarget);
    ItemNamePair QueryItemNamePair(const ItemID &kID);
    TaskName QueryTaskName(const TaskID &kID);
private:
    void AssertItemIDExisted(const ItemID &kID);
    // Check sql execute result. If it failed, throw ExecuteSQLFailedException.
    static void AssertSQLReturnCode(const std::string &kSQL, const SQLEXECUTERESULT_PTR &kSQLResult);
    static int QueryItemInInventory(const ItemIDAmountMap &kInventory, ItemID target);
};

#endif
