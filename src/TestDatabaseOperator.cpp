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
        db.AddItemInfo(ITEMINFO_PTR(new ItemInfo(ItemNamePair(std::string("item name"), std::string("type name")), 99))),
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
    ASSERT_NO_THROW(db.AddItemInfo(ITEMINFO_PTR(new ItemInfo(ItemNamePair(kItemName, kTypeName), kIDInGame_1))));
    ASSERT_THROW(db.AddItemInfo(ITEMINFO_PTR(new ItemInfo(ItemNamePair(kItemName, kTypeName), kIDInGame_2))), DatabaseInconsistencyException);
}

#endif
