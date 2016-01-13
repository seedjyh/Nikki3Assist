//
//   File name      : TestDatabaseOperator.cpp
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-11 15:57:00
//

#ifdef _GTEST_

// Implemented header
#include "DatabaseOperator.h"

// C system headers
// ...

// C++ system headers
// ...

// Headers from other projects
#include <gtest/gtest.h>

// Headers of current project
#include "DirectoryHolder.h"
#include "exception/DatabaseInconsistencyException.h"
#include "exception/NoSuchItemNameException.h"
#include "exception/NoSuchItemIDException.h"

static const Tstring s_kTestDirectoryPath(_T("test\\"));
static const Tstring s_kDatabaseFilePath = s_kTestDirectoryPath + _T("db.sqlite");

TEST(DatabaseOperator, DatabaseOperator)
{
    DirectoryHolder dir_holder(s_kTestDirectoryPath);
    ASSERT_NO_THROW(DatabaseOperator db(s_kDatabaseFilePath));
    ASSERT_NO_THROW(DatabaseOperator db(s_kDatabaseFilePath)); // load existed database
}

TEST(DatabaseOperator, AddItemInfo_NoSuchItemType)
{
    DirectoryHolder dir_holder(s_kTestDirectoryPath);
    DatabaseOperator db(s_kDatabaseFilePath);
    ASSERT_THROW(
        db.AddItemInfo(ITEMINFO_PTR(new ItemInfo(std::string("item name"), std::string("type name"), 99))),
        DatabaseInconsistencyException
    );
}

TEST(DatabaseOperator, AddItemInfo_DuplicateItemNameAndItemType)
{
    const std::string kItemName("item name");
    const std::string kTypeName("type name");
    const ItemIDInGame kIDInGame_1 = 1;
    const ItemIDInGame kIDInGame_2 = kIDInGame_1 + 1;
    DirectoryHolder dir_holder(s_kTestDirectoryPath);
    DatabaseOperator db(s_kDatabaseFilePath);
    ASSERT_NO_THROW(db.AddItemType(kTypeName));
    ASSERT_NO_THROW(db.AddItemInfo(ITEMINFO_PTR(new ItemInfo(kItemName, kTypeName, kIDInGame_1))));
    ASSERT_THROW(db.AddItemInfo(ITEMINFO_PTR(new ItemInfo(kItemName, kTypeName, kIDInGame_2))), DatabaseInconsistencyException);
}

TEST(DatabaseOperator, AddItemInfo_DuplicateIDInGame)
{
    const std::string kItemName_1("item name 1");
    const std::string kItemName_2("item name 2");
    const std::string kTypeName_1("type name 1");
    const std::string kTypeName_2("type name 2");
    const ItemIDInGame kIDInGame = 1;
    DirectoryHolder dir_holder(s_kTestDirectoryPath);
    DatabaseOperator db(s_kDatabaseFilePath);
    ASSERT_NO_THROW(db.AddItemType(kTypeName_1));
    ASSERT_NO_THROW(db.AddItemType(kTypeName_2));
    ASSERT_NO_THROW(db.AddItemInfo(ITEMINFO_PTR(new ItemInfo(kItemName_1, kTypeName_1, kIDInGame))));
    ASSERT_THROW(db.AddItemInfo(ITEMINFO_PTR(new ItemInfo(kItemName_2, kTypeName_2, kIDInGame))), DatabaseInconsistencyException);
}

TEST(DatabaseOperator, QueryItemID_QueryItemInfo)
{
    const std::string kItemName("item name");
    const std::string kTypeName("type name");
    const ItemIDInGame kIDInGame = 1;
    DirectoryHolder dir_holder(s_kTestDirectoryPath);
    DatabaseOperator db(s_kDatabaseFilePath);
    ASSERT_NO_THROW(db.AddItemType(kTypeName));
    ASSERT_NO_THROW(db.AddItemInfo(ITEMINFO_PTR(new ItemInfo(kItemName, kTypeName, kIDInGame))));
    ASSERT_THROW(db.QueryItemID(kItemName + "abc", kTypeName), NoSuchItemNameException);
    ASSERT_THROW(db.QueryItemID(kItemName, kTypeName + "def"), DatabaseInconsistencyException);
    ItemID item_id = 0;
    ASSERT_NO_THROW(item_id = db.QueryItemID(kItemName, kTypeName));

    ITEMINFO_PTR query_result_item_info;
    ASSERT_THROW(db.QueryItemInfo(item_id + 1), NoSuchItemIDException);
    ASSERT_NO_THROW(query_result_item_info = db.QueryItemInfo(item_id));

    ASSERT_STREQ(kItemName.c_str(), query_result_item_info->name().c_str());
    ASSERT_STREQ(kTypeName.c_str(), query_result_item_info->type_name().c_str());
    ASSERT_EQ(kIDInGame, query_result_item_info->id_in_game());
}

TEST(DatabaseOperator, DeleteItemInfo)
{
    const std::string kItemName("item name");
    const std::string kTypeName("type name");
    const ItemIDInGame kIDInGame = 1;
    DirectoryHolder dir_holder(s_kTestDirectoryPath);
    DatabaseOperator db(s_kDatabaseFilePath);

    // Prepare data
    ASSERT_NO_THROW(db.AddItemType(kTypeName));
    ASSERT_NO_THROW(db.AddItemInfo(ITEMINFO_PTR(new ItemInfo(kItemName, kTypeName, kIDInGame))));

    // try deleting
    const ItemID kItemID = db.QueryItemID(kItemName, kTypeName);
    ASSERT_NO_THROW(db.DeleteItemInfo(kItemID + 1)); // deleting not-existed item won't throw any exception
    ASSERT_NO_THROW(db.QueryItemInfo(kItemID)); // record is still there
    ASSERT_NO_THROW(db.DeleteItemInfo(kItemID)); // do delete with correct ID

    // check after deleting.
    ASSERT_THROW(db.QueryItemInfo(kItemID), NoSuchItemIDException);
    ASSERT_THROW(db.QueryItemID(kItemName, kTypeName), NoSuchItemNameException);
}

TEST(DatabaseOperator, UpdateItemInfo)
{
    const std::string kOldItemName("item name");
    const std::string kOldTypeName("type name");
    const ItemIDInGame kOldIDInGame = 1;

    const std::string kNewItemName = kOldItemName + " new";
    const std::string kNewTypeName = kOldTypeName + " new";
    const ItemIDInGame kNewIDInGame = kOldIDInGame + 1;

    DirectoryHolder dir_holder(s_kTestDirectoryPath);
    DatabaseOperator db(s_kDatabaseFilePath);

    // Prepare data
    ASSERT_NO_THROW(db.AddItemType(kOldTypeName));
    ASSERT_NO_THROW(db.AddItemInfo(ITEMINFO_PTR(new ItemInfo(kOldItemName, kOldTypeName, kOldIDInGame))));
    const ItemID kItemID = db.QueryItemID(kOldItemName, kOldTypeName);

    // Try updating with wrong ID.
    ASSERT_THROW(db.UpdateItemInfo(kItemID + 1, ITEMINFO_PTR(new ItemInfo(kNewItemName, kNewTypeName, kNewIDInGame))), NoSuchItemIDException);
    // Try updating with type-name not exists.
    ASSERT_THROW(db.UpdateItemInfo(kItemID, ITEMINFO_PTR(new ItemInfo(kNewItemName, kNewTypeName, kNewIDInGame))), DatabaseInconsistencyException);

    // Add new type name into the table ItemTypes.
    ASSERT_NO_THROW(db.AddItemType(kNewTypeName));
    ASSERT_NO_THROW(db.UpdateItemInfo(kItemID, ITEMINFO_PTR(new ItemInfo(kNewItemName, kNewTypeName, kNewIDInGame))));
    ITEMINFO_PTR new_info;
    ASSERT_NO_THROW(new_info = db.QueryItemInfo(kItemID));
    ASSERT_STREQ(kNewItemName.c_str(), new_info->name().c_str());
    ASSERT_STREQ(kNewTypeName.c_str(), new_info->type_name().c_str());
    ASSERT_EQ(kNewIDInGame, new_info->id_in_game());
}

#endif
