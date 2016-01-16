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
#include "exception/ExecuteSQLFailedException.h"
#include "exception/DatabaseInconsistencyException.h"
#include "exception/NoSuchItemNameException.h"
#include "exception/NoSuchItemIDException.h"
#include "exception/SystemErrorException.h"

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
    std::stringstream sql;
    sql << "select name from ItemTypes where name=\"" << kTypeName << "\"";
    SQLEXECUTERESULT_PTR result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), result);
    if (result->GetLineCount() > 0)
    {
        throw DatabaseInconsistencyException(std::string("ItemType \"") + kTypeName + "\" has already exist");
    }
    
    sql.str("");
    sql << "insert into ItemTypes(name) values(\"" << kTypeName << "\")";
    result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), result);

    return;
}

void DatabaseOperator::AddItemInfo(const ITEMINFO_PTR &kItemInfo)
{
    // query item-type-id, assure the existence of type-name.
    ItemTypeID type_id = QueryItemTypeID(kItemInfo->name_pair().type_name());

    // check duplication of type-name and item-name
    std::stringstream sql;
    sql << "select id from Items where ";
    sql << "    name = \"" << kItemInfo->name_pair().name() << "\" and";
    sql << "    type_id = " << type_id;
    SQLEXECUTERESULT_PTR result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), result);
    if (result->GetLineCount() > 0)
    {
        throw DatabaseInconsistencyException(std::string(
            "Item with name \"") + kItemInfo->name_pair().name() + "\"" +
            " and type-name \"" + kItemInfo->name_pair().type_name() + "\"" +
            " has already exist");
    }

    // check duplication of id-in-game
    sql.str("");
    sql << "select id from Items where id_in_game = " << kItemInfo->id_in_game();
    result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), result);
    if (result->GetLineCount() > 0)
    {
        std::stringstream ss;
        ss << "Item with id-in-game: " << kItemInfo->id_in_game() << " has already exist";
        throw DatabaseInconsistencyException(ss.str());
    }

    // insert record
    sql.str("");
    sql << "insert into Items(name, type_id, id_in_game) values(";
    sql << "    \"" << kItemInfo->name_pair().name() << "\", ";
    sql << "    " << type_id << ", ";
    sql << "    " << kItemInfo->id_in_game();
    sql << ")";
    result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), result);
    return;
}

ItemID DatabaseOperator::QueryItemID(const std::string &kItemName, const std::string &kTypeName)
{
    ItemTypeID type_id = QueryItemTypeID(kTypeName);
    std::stringstream sql;
    sql << "select id from Items where ";
    sql << "    name = \"" << kItemName << "\" and ";
    sql << "    type_id = " << type_id;
    SQLEXECUTERESULT_PTR result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), result);
    if (0 == result->GetLineCount())
    {
        throw NoSuchItemNameException(kItemName, kTypeName);
    }
    else if (result->GetLineCount() > 1)
    {
        throw DatabaseInconsistencyException(std::string("More than 1 records in table Items with same item-name: \"" + kItemName + "\" and item-type-name \"" + kTypeName + "\""));
    }
    assert(1 == result->GetLineCount());
    return NumberOperator::AtoI(result->GetResult(0, 0));
}

void DatabaseOperator::DeleteItemInfo(const ItemID &kID)
{
    std::stringstream sql;
    sql << "delete from Items where id = " << kID;
    SQLEXECUTERESULT_PTR result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), result);
    return;
}

void DatabaseOperator::UpdateItemInfo(const ItemID &kID, const ITEMINFO_PTR &kItemInfo)
{
    ITEMINFO_PTR old_info = QueryItemInfo(kID); // Confirm the existence of target record.
    const ItemTypeID kTypeID = QueryItemTypeID(kItemInfo->name_pair().type_name());

    std::stringstream sql;
    sql << "update Items set ";
    sql << "    name = \"" << kItemInfo->name_pair().name() << "\", ";
    sql << "    type_id = " << kTypeID << ", ";
    sql << "    id_in_game = " << kItemInfo->id_in_game() << " ";
    sql << "where id = " << kID;
    SQLEXECUTERESULT_PTR result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), result);

    return;
}

ITEMINFO_PTR DatabaseOperator::QueryItemInfo(const ItemID &kID)
{
    std::stringstream sql;
    sql << "select name, type_id, id_in_game from Items where id = " << kID;
    SQLEXECUTERESULT_PTR result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), result);
    if (0 == result->GetLineCount())
    {
        throw NoSuchItemIDException(kID);
    }
    else if (result->GetLineCount() > 1)
    {
        std::stringstream ss;
        ss << "More than 1 records in table Items with same id: " << kID;
        throw DatabaseInconsistencyException(ss.str());
    }
    assert(1 == result->GetLineCount());

    const std::string kItemName = result->GetResult(0, 0);
    const ItemTypeID kTypeID = NumberOperator::AtoI(result->GetResult(0, 1));
    const ItemIDInGame kIDInGame = NumberOperator::AtoI(result->GetResult(0, 2));
    const std::string kTypeName = QueryItemTypeName(kTypeID);
    return ITEMINFO_PTR(new ItemInfo(ItemNamePair(kItemName, kTypeName), kIDInGame));
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

const std::string DatabaseOperator::QueryItemTypeName(const ItemTypeID &kID)
{
    std::stringstream sql;
    sql << "select name from ItemTypes where id = " << kID;
    SQLEXECUTERESULT_PTR result = db_handler_->ExecuteSQL(sql.str());
    AssertSQLReturnCode(sql.str(), result);
    if (0 == result->GetLineCount())
    {
        std::stringstream ss;
        ss << "No required item-type with type-id: " << kID;
        throw DatabaseInconsistencyException(ss.str());
    }
    else if (result->GetLineCount() > 1)
    {
        std::stringstream ss;
        ss << "More than 1 records in table ItemTypes with same id: " << kID;
        throw DatabaseInconsistencyException(ss.str());
    }
    assert(1 == result->GetLineCount());

    return result->GetResult(0, 0);
}

//////////////////////////////////////////////////////////////////////////
void DatabaseOperator::AssertSQLReturnCode(const std::string &kSQL, const SQLEXECUTERESULT_PTR &kSQLResult)
{
    if (!kSQLResult->returned_code())
    {
        throw ExecuteSQLFailedException(kSQL, kSQLResult->returned_code());
    }
    return;
}
