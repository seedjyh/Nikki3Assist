//
//   File name      : DatabaseOperator.cpp
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-11 16:21:49
//

// Implemented header
#include "DatabaseOperator.h"

// C system headers
// ...

// C++ system headers
#include <sstream>

// Headers from other projects
// ...

// Headers of current project
#include "IFSQLiteAdaptor/SQLiteHandleFactory.h"
#include "IFOperator/CodeTransformer.h"
#include "IFOperator/NumberOperator.h"
#include "ItemIDAmount.h"
#include "exception/ExecuteSQLFailedException.h"
#include "exception/DatabaseInconsistencyException.h"
#include "exception/NoSuchItemNameException.h"
#include "exception/NoSuchItemIDException.h"
#include "exception/SystemErrorException.h"
#include "exception/DuplicateTaskException.h"
#include "exception/NoSuchTaskException.h"
#include "exception/NoWayToAcquireItemException.h"

DatabaseOperator::DatabaseOperator(const Tstring &kFilePath)
:   db_handler_(SQLiteHandleFactory::Create())
{
    const std::string kFilePath_str = CodeTransformer::TransTstringToString(kFilePath);
    if (!db_handler_->Open(kFilePath_str))
    {
        throw SystemErrorException(std::string("Database"), std::string("Open database file \"") + kFilePath_str + "\" failed");
    }

    if (
        (!CheckTablesIntegrity()) || 
        (!CheckDataConsistency())
    )
    {
        DeleteAllTables();
        CreateEmptyTables();
        CreateTriggers();
    }
}

void DatabaseOperator::AddItemType(const std::string &kTypeName)
{
    if (CheckItemTypeExistence(kTypeName))
    {
        throw DatabaseInconsistencyException(std::string("ItemType \"") + kTypeName + "\" has already exist");
    }

    InsertItemType(kTypeName);

    return;
}

void DatabaseOperator::AddItemInfo(const ITEMINFO_PTR &kItemInfo)
{
    if (CheckItemInfoExistence(kItemInfo->name_pair()))
    {
        throw DatabaseInconsistencyException(std::string(
            "Item with name \"") + kItemInfo->name_pair().name() + "\"" +
            " and type-name \"" + kItemInfo->name_pair().type_name() + "\"" +
            " has already exist");
    }

    InsertItemInfo(QueryItemTypeID(kItemInfo->name_pair().type_name()), kItemInfo);
    return;
}

void DatabaseOperator::SetItemPrice(const ItemNamePair &kItemNamePair, const ItemAmountList &kPrice)
{
    ItemID good_id = QueryItemID(kItemNamePair);
    ItemIDAmountMap all_money;
    for (ItemAmountList::const_iterator ite = kPrice.begin(); ite != kPrice.end(); ++ite)
    {
        ItemID money_id = QueryItemID(ite->name_pair());
        all_money[money_id] += ite->count();
    }
    DeleteItemPrice(good_id);
    InsertItemPrice(good_id, all_money);

    return;
}

void DatabaseOperator::SetItemStock(const ItemNamePair &kItemNamePair, int count)
{
    ItemID item_id = QueryItemID(kItemNamePair);
    DeleteItemStock(item_id);
    InsertItemStock(item_id, count);
    return;
}

void DatabaseOperator::SetItemCreatingRule(const ItemNamePair &kProductItemNamePair, const ItemAmountList &kRawMaterials)
{
    ItemID product_id = QueryItemID(kProductItemNamePair);
    ItemIDAmountMap materials;
    for (ItemAmountList::const_iterator ite = kRawMaterials.begin(); ite != kRawMaterials.end(); ++ite)
    {
        ItemID raw_material_id = QueryItemID(ite->name_pair());
        materials[raw_material_id] += ite->count();
    }
    DeleteItemCreatingRule(product_id);
    InsertItemCreatingRule(product_id, materials);
    return;
}

void DatabaseOperator::SetItemDyeingRule(const ItemNamePair &kProductItemNamePair, const ItemAmountList &kRawMaterials)
{
    ItemID product_id = QueryItemID(kProductItemNamePair);
    ItemIDAmountMap materials;
    for (ItemAmountList::const_iterator ite = kRawMaterials.begin(); ite != kRawMaterials.end(); ++ite)
    {
        ItemID raw_material_id = QueryItemID(ite->name_pair());
        materials[raw_material_id] += ite->count();
    }
    DeleteItemDyeingRule(product_id);
    InsertItemDyeingRule(product_id, materials);
    return;
}

void DatabaseOperator::AddTask(const TaskName &kTaskName)
{
    if (CheckTaskExistence(kTaskName))
    {
        throw DuplicateTaskException(kTaskName);
    }
    InsertTask(kTaskName);
    return;
}

void DatabaseOperator::AddTaskReward(const TaskName &kTaskName, const ItemAmountList &kRewards)
{
    TaskID task_id = QueryTaskID(kTaskName);
    TaskRewards rewards;
    for (ItemAmountList::const_iterator ite = kRewards.begin(); ite != kRewards.end(); ++ite)
    {
        rewards.insert(QueryItemID(ite->name_pair()));
    }
    DeleteTaskRewards(task_id);
    InsertTaskRewards(task_id, rewards);
    
    return;
}

static void operator+=(ItemIDAmountMap &left, const ItemIDAmountMap &right)
{
    for (ItemIDAmountMap::const_iterator ite = right.begin(); ite != right.end(); ++ite)
    {
        left[ite->first] += ite->second;
    }
    return;
}

ITEMACQUISITION_PTR DatabaseOperator::QueryItemAcquisitionMean(const ItemAmountList &kTargets)
{
    ItemIDAmountMap current_inventory = QueryAllItemInventory();
    ItemIDAmountMap required_extra_basic_items;
    for (ItemAmountList::const_iterator ite = kTargets.begin(); ite != kTargets.end(); ++ite)
    {
        //  DFS
        TryMakeItem(ItemIDAmount(QueryItemID(ite->name_pair()), ite->count()), current_inventory, required_extra_basic_items);
    }
    // Scan items need to acquire and items can't be acquire, generate result.
    return QueryBasicItemAcquisitionMean(required_extra_basic_items);
}

//////////////////////////////////////////////////////////////////////////
void DatabaseOperator::CreateEmptyTables()
{
    // table: ItemTypes
    std::string sql(
        "CREATE TABLE ItemTypes("
        "   id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
        "   name TEXT NOT NULL"
        ")"
    );
    SQLEXECUTERESULT_PTR sql_result = db_handler_->ExecuteSQL(sql);
    AssertSQLReturnCode(sql, sql_result);

    // table: Items
    sql.assign(
        "CREATE TABLE Items("
        "   id INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,"
        "   name TEXT NOT NULL,"
        "   type_id INTEGER NOT NULL,"
        "   id_in_game INTEGER NOT NULL"
        ")"
    );
    sql_result = db_handler_->ExecuteSQL(sql);
    AssertSQLReturnCode(sql, sql_result);
    
    // table: Inventory
    sql.assign(
        "CREATE TABLE Inventory("
        "   item_id INTEGER PRIMARY KEY NOT NULL,"
        "   count TEXT NOT NULL"
        ")"
    );
    sql_result = db_handler_->ExecuteSQL(sql);
    AssertSQLReturnCode(sql, sql_result);

    // table: ItemsPrice
    sql.assign(
        "CREATE TABLE ItemsPrice("
        "   item_id INTEGER NOT NULL,"
        "   money_id INTEGER NOT NULL,"
        "   price INTEGER NOT NULL"
        ")"
    );
    sql_result = db_handler_->ExecuteSQL(sql);
    AssertSQLReturnCode(sql, sql_result);

    // table: CreatingRules
    sql.assign(
        "CREATE TABLE CreatingRules("
        "   product_id INTEGER NOT NULL,"
        "   raw_material_id INTEGER NOT NULL,"
        "   count INTEGER NOT NULL"
        ")"
    );
    sql_result = db_handler_->ExecuteSQL(sql);
    AssertSQLReturnCode(sql, sql_result);

    // table: DyeingRules
    sql.assign(
        "CREATE TABLE DyeingRules("
        "   product_id INTEGER NOT NULL,"
        "   raw_material_id INTEGER NOT NULL,"
        "   count INTEGER NOT NULL"
        ")"
    );
    sql_result = db_handler_->ExecuteSQL(sql);
    AssertSQLReturnCode(sql, sql_result);

    // table: Tasks
    sql.assign(
        "CREATE TABLE Tasks("
        "   id INTEGER PRIMARY KEY NOT NULL,"
        "   type TEXT NOT NULL,"
        "   chapter TEXT NOT NULL,"
        "   stage TEXT NOT NULL"
        ")"
    );
    sql_result = db_handler_->ExecuteSQL(sql);
    AssertSQLReturnCode(sql, sql_result);

    // table: TaskRewards
    sql.assign(
        "CREATE TABLE TaskRewards("
        "   task_id INTEGER NOT NULL,"
        "   reward_item_id INTEGER NOT NULL"
        ")"
    );
    sql_result = db_handler_->ExecuteSQL(sql);
    AssertSQLReturnCode(sql, sql_result);

    return;
}

void DatabaseOperator::CreateTriggers()
{
    // TODO: When a record is inserted into table Items, there should be a new record inserted into table Inventory wtih count 0.
    // TODO: When a record in table Items was deleted, records in any tables referring to the ID of this item should be deleted
    //       If one record X referred to this ID as an raw material in some kind of "Rule" table,
    //       all records sharing product ID with record X should be deleted too,
    //       because they are logically in the same "rule".
    //       It could be done automatically by triggers.
    return;
}

void DatabaseOperator::DeleteAllTables()
{
    db_handler_->ExecuteSQL(std::string("DROP TABLE ItemTypes"));
    db_handler_->ExecuteSQL(std::string("DROP TABLE Items"));
    return;
}

bool DatabaseOperator::CheckTablesIntegrity()
{
    try
    {
        // table: ItemTypes
        std::string sql("select id, name from ItemTypes");
        SQLEXECUTERESULT_PTR sql_result = db_handler_->ExecuteSQL(sql);
        AssertSQLReturnCode(sql, sql_result);

        // table: Items
        sql.assign("select id, name, type_id, id_in_game from Items");
        sql_result = db_handler_->ExecuteSQL(sql);
        AssertSQLReturnCode(sql, sql_result);
    }
    catch (ExecuteSQLFailedException &)
    {
        return false;
    }
    return true;
}

//////////////////////////////////////////////////////////////////////////
ItemTypeID DatabaseOperator::QueryItemTypeID(const std::string &kItemTypeName)
{
    std::string sql = std::string("select id from ItemTypes where name = \"") + kItemTypeName + "\"";
    SQLEXECUTERESULT_PTR sql_result = db_handler_->ExecuteSQL(sql);
    AssertSQLReturnCode(sql, sql_result);
    if (0 == sql_result->GetLineCount())
    {
        throw DatabaseInconsistencyException(std::string("No such item type: ") + kItemTypeName);
    }
    else if (sql_result->GetLineCount() > 1)
    {
        throw DatabaseInconsistencyException(std::string("more than 1 records in table ItemTypes with item-type-name: \"") + kItemTypeName + "\"");
    }
    assert(1 == sql_result->GetLineCount());
    std::string id_in_string = sql_result->GetResult(0, 0);
    ItemTypeID id = 0;
    if (sscanf(id_in_string.c_str(), "%d", &id) != 1)
    {
        throw DatabaseInconsistencyException(std::string("item type id is NaN: ") + id_in_string);
    }
    return id;
}

ItemID DatabaseOperator::QueryItemID(const ItemNamePair &kItemNamePair)
{
    ItemTypeID type_id = QueryItemTypeID(kItemNamePair.type_name());
    std::stringstream sql;
    sql << "select id from Items where ";
    sql << "    name = \"" << kItemNamePair.name() << "\" and ";
    sql << "    type_id = " << type_id;
    SQLEXECUTERESULT_PTR result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), result);
    if (0 == result->GetLineCount())
    {
        throw NoSuchItemNameException(kItemNamePair);
    }
    else if (result->GetLineCount() > 1)
    {
        throw DatabaseInconsistencyException(std::string("More than 1 records in table Items with same item-name: \"" + kItemNamePair.name() + "\" and item-type-name \"" + kItemNamePair.type_name() + "\""));
    }
    assert(1 == result->GetLineCount());
    return NumberOperator::AtoI(result->GetResult(0, 0));
}

bool DatabaseOperator::CheckItemTypeExistence(const std::string &kItemTypeName)
{
    std::stringstream sql;
    sql << "select name from ItemTypes where name=\"" << kItemTypeName << "\"";
    SQLEXECUTERESULT_PTR result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), result);
    return (result->GetLineCount() > 0);
}

void DatabaseOperator::InsertItemType(const std::string &kItemTypeName)
{
    std::stringstream sql;
    sql << "insert into ItemTypes(name) values(\"" << kItemTypeName << "\")";
    SQLEXECUTERESULT_PTR result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), result);
    return;
}

bool DatabaseOperator::CheckItemInfoExistence(const ItemNamePair &kItemNamePair)
{
    std::stringstream sql;
    sql << "select Items.name from Items, ItemTypes where Items.type_id = ItemTypes.id and Items.name=\"" << kItemNamePair.name() << "\" and ItemTypes.name=\"" << kItemNamePair.type_name() << "\"";
    SQLEXECUTERESULT_PTR result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), result);
    return (result->GetLineCount() > 0);
}

void DatabaseOperator::InsertItemInfo(ItemTypeID type_id, const ITEMINFO_PTR &kItemInfo)
{
    std::stringstream sql;
    sql << "insert into Items(name, type_id, id_in_game) values(";
    sql << "    \"" << kItemInfo->name_pair().name() << "\", ";
    sql << "    " << type_id << ", ";
    sql << "    " << kItemInfo->id_in_game();
    sql << ")";
    SQLEXECUTERESULT_PTR result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), result);
    return;
}

int DatabaseOperator::QueryItemStock(ItemID item_id)
{
    std::stringstream sql;
    sql << "select count from Inventory where item_id = " << item_id;
    SQLEXECUTERESULT_PTR result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), result);
    if (0 == result->GetLineCount())
    {
        return 0;
    }
    assert(1 == result->GetLineCount()); // item_id is primery key of table Inventory

    return NumberOperator::AtoI(result->GetResult(0, 0));
}

bool DatabaseOperator::CheckItemIDExistence(ItemID item_id)
{
    std::stringstream sql;
    sql << "select id from Items where id = " << item_id;
    SQLEXECUTERESULT_PTR result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), result);
    return (result->GetLineCount() > 0);
}

void DatabaseOperator::DeleteItemPrice(ItemID item_id)
{
    std::stringstream sql;
    sql << "delete from ItemsPrice where item_id = " << item_id;
    SQLEXECUTERESULT_PTR result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), result);
    return;
}

void DatabaseOperator::InsertItemPrice(const ItemID &kID, const ItemIDAmountMap &kPrice)
{
    for (ItemIDAmountMap::const_iterator ite = kPrice.begin(); ite != kPrice.end(); ++ite)
    {
        std::stringstream sql;
        sql << "insert into ItemsPrice(item_id, money_id, price) values (" << kID << ", " << ite->first << ", " << ite->second << ")";
        SQLEXECUTERESULT_PTR result = db_handler_->ExecuteSQL(sql.str());
        AssertSQLReturnCode(sql.str(), result);
    }
    return;
}

void DatabaseOperator::DeleteItemStock(ItemID item_id)
{
    std::stringstream sql;
    sql << "delete from Inventory where item_id = " << item_id;
    SQLEXECUTERESULT_PTR result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), result);
    return;
}

void DatabaseOperator::InsertItemStock(ItemID item_id, int count)
{
    std::stringstream sql;
    sql << "insert into Inventory(item_id, count) values (" << item_id << ", " << count << ")";
    SQLEXECUTERESULT_PTR result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), result);
    return;
}

void DatabaseOperator::DeleteItemCreatingRule(ItemID product_id)
{
    std::stringstream sql;
    sql << "delete from CreatingRules where product_id = " << product_id;
    SQLEXECUTERESULT_PTR result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), result);
    return;
}

void DatabaseOperator::InsertItemCreatingRule(ItemID product_id, const ItemIDAmountMap &kMaterials)
{
    for (ItemIDAmountMap::const_iterator ite = kMaterials.begin(); ite != kMaterials.end(); ++ite)
    {
        std::stringstream sql;
        sql << "insert into CreatingRules(product_id, raw_material_id, count) values (" << product_id << ", " << ite->first << ", " << ite->second << ")";
        SQLEXECUTERESULT_PTR result = db_handler_->ExecuteSQL(sql.str());
        AssertSQLReturnCode(sql.str(), result);
    }
    return;
}

void DatabaseOperator::DeleteItemDyeingRule(ItemID product_id)
{
    std::stringstream sql;
    sql << "delete from DyeingRules where product_id = " << product_id;
    SQLEXECUTERESULT_PTR result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), result);
    return;
}

void DatabaseOperator::InsertItemDyeingRule(ItemID product_id, const ItemIDAmountMap &kMaterials)
{
    for (ItemIDAmountMap::const_iterator ite = kMaterials.begin(); ite != kMaterials.end(); ++ite)
    {
        std::stringstream sql;
        sql << "insert into DyeingRules(product_id, raw_material_id, count) values (" << product_id << ", " << ite->first << ", " << ite->second << ")";
        SQLEXECUTERESULT_PTR result = db_handler_->ExecuteSQL(sql.str());
        AssertSQLReturnCode(sql.str(), result);
    }
    return;
}

bool DatabaseOperator::CheckTaskExistence(const TaskName &kTaskName)
{
    std::stringstream sql;
    sql << "select id from Tasks where type = \"" << kTaskName.type() << "\" and chapter = \"" << kTaskName.chapter() << "\" and stage = \"" + kTaskName.stage() << "\"";
    SQLEXECUTERESULT_PTR result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), result);
    return (result->GetLineCount() > 0);
}

void DatabaseOperator::InsertTask(const TaskName &kTaskName)
{
    std::stringstream sql;
    sql << "insert into Tasks(type, chapter, stage) values (\"" << kTaskName.type() << "\", \"" << kTaskName.chapter() << "\", \"" + kTaskName.stage() << "\")";
    SQLEXECUTERESULT_PTR result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), result);
    return;
}

TaskID DatabaseOperator::QueryTaskID(const TaskName &kTaskName)
{
    std::stringstream sql;
    sql << "select id from Tasks where type = \"" << kTaskName.type() << "\" and chapter = \"" << kTaskName.chapter() << "\" and stage = \"" + kTaskName.stage() << "\"";
    SQLEXECUTERESULT_PTR result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), result);
    if (result->GetLineCount() <= 0)
    {
        throw NoSuchTaskException(kTaskName);
    }
    return NumberOperator::AtoI(result->GetResult(0, 0));
}

void DatabaseOperator::DeleteTaskRewards(TaskID task_id)
{
    std::stringstream sql;
    sql << "delete from TaskRewards where task_id = " << task_id;
    SQLEXECUTERESULT_PTR result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), result);
    return;
}

void DatabaseOperator::InsertTaskRewards(TaskID task_id, const TaskRewards &kRewards)
{
    for (TaskRewards::const_iterator ite = kRewards.begin(); ite != kRewards.end(); ++ite)
    {
        std::stringstream sql;
        sql << "insert into TaskRewards(task_id, reward_item_id) values (\"" << task_id << "\", " << *ite << ")";
        SQLEXECUTERESULT_PTR result = db_handler_->ExecuteSQL(sql.str());
        AssertSQLReturnCode(sql.str(), result);
    }
    return;
}

ItemIDAmountMap DatabaseOperator::QueryAllItemInventory()
{
    ItemIDAmountMap result;
    std::string sql("select item_id, count from Inventory");
    SQLEXECUTERESULT_PTR sql_result = db_handler_->ExecuteSQL(sql);
    AssertSQLReturnCode(sql, sql_result);
    for (int i = 0; i < sql_result->GetLineCount(); i++)
    {
        const ItemID kItemID = NumberOperator::AtoI(sql_result->GetResult(i, 0));
        const int kCount = NumberOperator::AtoI(sql_result->GetResult(i, 1));
        assert(result.end() == result.find(kItemID)); // item_id should be unique in table Inventory.
        result[kItemID] = kCount;
    }

    return result;
}

// FUNCTION: Assume acquire all of target items.
// @target: item to get
// @count: number of items to get
// @i_o_inventory:
//      before call:all items we have got now.
//      after call: all items we have got now, including @count of @target
// @i_o_required_extra_basic_items:
//      before call: all basic items we need now, except getting @count of @target
//      after call: all basic items we need, including getting @count of @target
// NOTE: fee for creating, dyeing or evolution would not be count here.
void DatabaseOperator::TryMakeItem(
    const ItemIDAmount &kTarget,
    ItemIDAmountMap &i_o_inventory,
    ItemIDAmountMap &i_o_required_extra_basic_items
)
{
    const int kTargetAmountInInventory = QueryItemInInventory(i_o_inventory, kTarget.id());
    if (kTargetAmountInInventory >= kTarget.count()) // Already have got enough.
    {
        return;
    }
    const int kExtraTargetNeedToGet = kTarget.count() - kTargetAmountInInventory;

    // Check task first
    {
        TaskIDSet useful_tasks = QueryTaskToGetTargetItem(kTarget.id());
        if (!useful_tasks.empty())
        {
            i_o_inventory[kTarget.id()] = kTarget.count();
            i_o_required_extra_basic_items[kTarget.id()] += kExtraTargetNeedToGet;
            return;
        }
    }

    // Then check store
    {
        ItemIDAmountMap money_need_to_buy = QueryMoneyToBuyTargetItem(ItemIDAmount(kTarget.id(), kExtraTargetNeedToGet));
        if (!money_need_to_buy.empty())
        {
            i_o_inventory[kTarget.id()] = kTarget.count();
            i_o_required_extra_basic_items[kTarget.id()] += kExtraTargetNeedToGet;
            return;
        }
    }

    // Then check creating
    {
        ItemIDAmountMap materials_for_creating = QueryMaterialsForCreating(ItemIDAmount(kTarget.id(), kExtraTargetNeedToGet));
        if (!materials_for_creating.empty())
        {
            for (ItemIDAmountMap::const_iterator ite = materials_for_creating.begin(); ite != materials_for_creating.end(); ++ite)
            {
                TryMakeItem(ItemIDAmount(ite->first, ite->second), i_o_inventory, i_o_required_extra_basic_items);
            }
            return;
        }
    }

    // Then check dyeing
    {
        ItemIDAmountMap materials_for_dyeing = QueryMaterialsForDyeing(ItemIDAmount(kTarget.id(), kExtraTargetNeedToGet));
        if (!materials_for_dyeing.empty())
        {
            for (ItemIDAmountMap::const_iterator ite = materials_for_dyeing.begin(); ite != materials_for_dyeing.end(); ++ite)
            {
                TryMakeItem(ItemIDAmount(ite->first, ite->second), i_o_inventory, i_o_required_extra_basic_items);
            }
            return;
        }
    }

    AssertItemIDExisted(kTarget.id());
    throw NoWayToAcquireItemException(QueryItemNamePair(kTarget.id()));
    return;
}

ITEMACQUISITION_PTR DatabaseOperator::QueryBasicItemAcquisitionMean(const ItemIDAmountMap &kTargets)
{
    //////////////////////////////////////////////////////////////////////////
    typedef std::map<TaskID, ItemIDAmountMap> TaskIDItemIDAmountMap;
    typedef struct  
    {
        int money_count;
        ItemIDAmountMap items_to_buy;
    }PriceForItems;
    typedef std::map<ItemID, PriceForItems> MoneyIDItemIDAmountMap;
    //////////////////////////////////////////////////////////////////////////
    TaskIDItemIDAmountMap task_mean;
    MoneyIDItemIDAmountMap money_mean;
    //////////////////////////////////////////////////////////////////////////
    ITEMACQUISITION_PTR result(new ItemAcquisition());
    for (ItemIDAmountMap::const_iterator item_ite = kTargets.begin(); item_ite != kTargets.end(); ++item_ite)
    {
        // task
        TaskIDSet useful_tasks = QueryTaskToGetTargetItem(item_ite->first);
        if (!useful_tasks.empty())
        {
            for (TaskIDSet::const_iterator task_ite = useful_tasks.begin(); task_ite != useful_tasks.end(); ++task_ite)
            {
                task_mean[*task_ite][item_ite->first] += item_ite->second;
            }
            continue;
        }

        // money
        ItemIDAmountMap money = QueryMoneyToBuyTargetItem(ItemIDAmount(item_ite->first, item_ite->second));
        if (!money.empty())
        {
            if (money.size() != 1)
            {
                throw DatabaseInconsistencyException(std::string("Multi-price for one item: ") + QueryItemNamePair(item_ite->first).str());
            }
            ItemID money_id = money.begin()->first;
            int money_count = money.begin()->second;
            money_mean[money_id].money_count += money_count;
            assert(money_mean[money_id].items_to_buy.end() == money_mean[money_id].items_to_buy.find(item_ite->first));
            money_mean[money_id].items_to_buy[item_ite->first] = item_ite->second;
            continue;
        }

        // or throw exception
        throw NoWayToAcquireItemException(QueryItemNamePair(item_ite->first));
    }
    //////////////////////////////////////////////////////////////////////////
    // generate text version
    for (TaskIDItemIDAmountMap::const_iterator task_ite = task_mean.begin(); task_ite != task_mean.end(); ++task_ite)
    {
        TaskName task_name = QueryTaskName(task_ite->first);
        ItemAmountList items_get_from_this_task;
        for (ItemIDAmountMap::const_iterator item_ite = task_ite->second.begin(); item_ite != task_ite->second.end(); ++item_ite)
        {
            items_get_from_this_task.push_back(ItemAmount(QueryItemNamePair(item_ite->first), item_ite->second));
        }
        result->AppendTaskToDo(TaskToDo(task_name, items_get_from_this_task));
    }
    for (MoneyIDItemIDAmountMap::const_iterator money_ite = money_mean.begin(); money_ite != money_mean.end(); ++money_ite)
    {
        ItemAmount money_name_amount(QueryItemNamePair(money_ite->first), money_ite->second.money_count);
        ItemAmountList items_bought_with_this_money;
        for (ItemIDAmountMap::const_iterator item_ite = money_ite->second.items_to_buy.begin(); item_ite != money_ite->second.items_to_buy.end(); ++item_ite)
        {
            items_bought_with_this_money.push_back(ItemAmount(QueryItemNamePair(item_ite->first), item_ite->second));
        }

        result->AppendMoneyToPay(MoneyToPay(money_name_amount, items_bought_with_this_money));
    }
    return result;
}

TaskIDSet DatabaseOperator::QueryTaskToGetTargetItem(ItemID target)
{
    TaskIDSet result;
    std::stringstream sql;
    sql << "select task_id from TaskRewards where reward_item_id = " << target;
    SQLEXECUTERESULT_PTR sql_result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), sql_result);
    for (int i = 0; i < sql_result->GetLineCount(); i++)
    {
        result.insert(NumberOperator::AtoI(sql_result->GetResult(i, 0)));
    }
    return result;
}

ItemIDAmountMap DatabaseOperator::QueryMoneyToBuyTargetItem(const ItemIDAmount &kTarget)
{
    ItemIDAmountMap money;
    std::stringstream sql;
    sql << "select money_id, price from ItemsPrice where item_id = " << kTarget.id();
    SQLEXECUTERESULT_PTR sql_result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), sql_result);
    for (int i = 0; i < sql_result->GetLineCount(); i++)
    {
        int money_id = NumberOperator::AtoI(sql_result->GetResult(i, 0));
        int price = NumberOperator::AtoI(sql_result->GetResult(i, 1));
        assert(money.end() == money.find(money_id));
        money[money_id] = price * kTarget.count();
    }
    return money;
}

ItemIDAmountMap DatabaseOperator::QueryMaterialsForCreating(const ItemIDAmount &kTarget)
{
    ItemIDAmountMap materials;
    std::stringstream sql;
    sql << "select raw_material_id, count from CreatingRules where product_id = " << kTarget.id();
    SQLEXECUTERESULT_PTR sql_result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), sql_result);
    for (int i = 0; i < sql_result->GetLineCount(); i++)
    {
        ItemID material_id = NumberOperator::AtoI(sql_result->GetResult(i, 0));
        int material_count = NumberOperator::AtoI(sql_result->GetResult(i, 1));
        assert(materials.end() == materials.find(material_id));
        materials[material_id] = (material_count - 1) * (kTarget.count() - 1) + material_count; // creating (or revolution) would left 1 material after processing.
    }
    return materials;
}

ItemIDAmountMap DatabaseOperator::QueryMaterialsForDyeing(const ItemIDAmount &kTarget)
{
    ItemIDAmountMap materials;
    std::stringstream sql;
    sql << "select raw_material_id, count from DyeingRules where product_id = " << kTarget.id();
    SQLEXECUTERESULT_PTR sql_result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), sql_result);
    for (int i = 0; i < sql_result->GetLineCount(); i++)
    {
        ItemID material_id = NumberOperator::AtoI(sql_result->GetResult(i, 0));
        int material_count = NumberOperator::AtoI(sql_result->GetResult(i, 1));
        assert(materials.end() == materials.find(material_id));
        materials[material_id] = material_count * kTarget.count(); // dyeing would consume all materials
    }
    return materials;
}

ItemNamePair DatabaseOperator::QueryItemNamePair(const ItemID &kID)
{
    std::stringstream sql;
    sql << "select Items.name, ItemTypes.name from Items, ItemTypes where Items.type_id = ItemTypes.id and Items.id = " << kID;
    SQLEXECUTERESULT_PTR sql_result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), sql_result);
    assert(1 == sql_result->GetLineCount());
    return ItemNamePair(sql_result->GetResult(0, 0), sql_result->GetResult(0, 1));
}

TaskName DatabaseOperator::QueryTaskName(const TaskID &kID)
{
    std::stringstream sql;
    sql << "select type, chapter, stage from Tasks where id = " << kID;
    SQLEXECUTERESULT_PTR sql_result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), sql_result);
    assert(1 == sql_result->GetLineCount());
    return TaskName(
        sql_result->GetResult(0, 0),
        sql_result->GetResult(0, 1),
        sql_result->GetResult(0, 2)
    );
}

//////////////////////////////////////////////////////////////////////////
void DatabaseOperator::AssertItemIDExisted(const ItemID &kID)
{
    if (!CheckItemIDExistence(kID))
    {
        std::stringstream ss;
        ss << "Item ID " << kID << " does not exist.";
        throw DatabaseInconsistencyException(ss.str());
    }
    return;
}

void DatabaseOperator::AssertSQLReturnCode(const std::string &kSQL, const SQLEXECUTERESULT_PTR &kSQLResult)
{
    if (!kSQLResult->returned_code())
    {
        throw ExecuteSQLFailedException(kSQL, kSQLResult->returned_code());
    }
    return;
}

int DatabaseOperator::QueryItemInInventory(const ItemIDAmountMap &kInventory, ItemID target)
{
    ItemIDAmountMap::const_iterator ite = kInventory.find(target);
    if (kInventory.end() == ite)
    {
        return 0;
    }
    return ite->second;
}
