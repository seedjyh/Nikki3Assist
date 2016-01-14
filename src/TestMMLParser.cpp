//
//   File name      : TestMMLParser.cpp
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-14 10:09:30
//

#ifdef _GTEST_

// Implemented header
#include "MMLParser.h"

// C system headers
// ...

// C++ system headers
// ...

// Headers from other projects
#include <gtest/gtest.h>

// Headers of current project
#include "exception/MMLFormatErrorException.h"
#include "exception/NoSuchArgumentException.h"

TEST(MMLParser, CheckCommandName_NoArgument)
{
    MMLParser parser(std::string("ADD"));
    ASSERT_TRUE(parser.CheckCommandName(std::string("ADD:;")));
    ASSERT_TRUE(parser.CheckCommandName(std::string("ADD:a=1")));
    ASSERT_THROW(parser.CheckCommandName(std::string("ADD")), MMLFormatErrorException);
}

TEST(MMLParser, Parse_WrongArgumentType)
{
    MMLParser parser = MMLParser(std::string("ADD"))
        (std::string("FIRST"), MMLParser::eArgumentType_Integer)
        (std::string("SECOND"), MMLParser::eArgumentType_Integer);
    ASSERT_THROW(parser.Parse(std::string("ADD:FIRST=5,SECOND=\"7\"")), MMLFormatErrorException);
}

TEST(MMLParser, Parse_Integer)
{
    MMLParser parser = MMLParser(std::string("ADD"))
        (std::string("FIRST"), MMLParser::eArgumentType_Integer)
        (std::string("SECOND"), MMLParser::eArgumentType_Integer);
    MMLCOMMAND_PTR command;
    ASSERT_NO_THROW(command = parser.Parse(std::string("ADD:FIRST=5,SECOND=7;")));
    ASSERT_STREQ("ADD", command->name().c_str());
    ASSERT_EQ(5, command->arguments().GetInteger(std::string("FIRST")));
    ASSERT_EQ(7, command->arguments().GetInteger(std::string("SECOND")));
    ASSERT_THROW(command->arguments().GetInteger(std::string("THIRD")), NoSuchArgumentException);
    ASSERT_THROW(command->arguments().GetString(std::string("FIRST")), NoSuchArgumentException);
    ASSERT_THROW(command->arguments().GetItemNamePair(std::string("FIRST")), NoSuchArgumentException);
    ASSERT_THROW(command->arguments().GetItemAmountList(std::string("FIRST")), NoSuchArgumentException);
}

TEST(MMLParser, Parse_String)
{
    MMLParser parser = MMLParser(std::string("STRCAT"))
        (std::string("FIRST"), MMLParser::eArgumentType_String)
        (std::string("SECOND"), MMLParser::eArgumentType_String);
    MMLCOMMAND_PTR command;
    ASSERT_THROW(command = parser.Parse(std::string("STRCAT:FIRST=\"abc\", SECOND=456;")), MMLFormatErrorException);
    ASSERT_THROW(command = parser.Parse(std::string("STRCAT:FIRST=\"abc\", SECOND=def;")), MMLFormatErrorException);
    ASSERT_NO_THROW(command = parser.Parse(std::string("STRCAT:FIRST=\"abc\", SECOND=\"def\";")));
    ASSERT_STREQ("STRCAT", command->name().c_str());
    ASSERT_STREQ("abc", command->arguments().GetString(std::string("FIRST")).c_str());
    ASSERT_STREQ("def", command->arguments().GetString(std::string("SECOND")).c_str());
}

TEST(MMLParser, Parse_ItemNamePair)
{
    MMLParser parser = MMLParser(std::string("ADD-ITEM"))
        (std::string("FIRST"), MMLParser::eArgumentType_ItemNamePair)
        (std::string("SECOND"), MMLParser::eArgumentType_ItemNamePair);
    MMLCOMMAND_PTR command;
    ASSERT_THROW(command = parser.Parse(std::string("ADD-ITEM:FIRST=(a|b),SECOND=(c|d);")), MMLFormatErrorException);
    ASSERT_THROW(command = parser.Parse(std::string("ADD-ITEM:FIRST=(\"a|b\"),SECOND=(\"c|d\");")), MMLFormatErrorException);
    ASSERT_THROW(command = parser.Parse(std::string("ADD-ITEM:FIRST=\"(a|b)\",SECOND=\"(c|d)\";")), MMLFormatErrorException);
    ASSERT_NO_THROW(command = parser.Parse(std::string("ADD-ITEM:FIRST=(\"a\"|\"b\"),SECOND=(\"c\"|\"d\");")));
    ASSERT_STREQ("ADD-ITEM", command->name().c_str());
    ASSERT_STREQ("a", command->arguments().GetItemNamePair(std::string("FIRST")).name().c_str());
    ASSERT_STREQ("b", command->arguments().GetItemNamePair(std::string("FIRST")).type_name().c_str());
    ASSERT_STREQ("c", command->arguments().GetItemNamePair(std::string("SECOND")).name().c_str());
    ASSERT_STREQ("d", command->arguments().GetItemNamePair(std::string("SECOND")).type_name().c_str());
}

TEST(MMLParser, Parse_ItemAmountList)
{
    MMLParser parser = MMLParser(std::string("ADD-RULE"))
        (std::string("PRODUCT"), MMLParser::eArgumentType_ItemAmountList)
        (std::string("RAW_MATERIAL"), MMLParser::eArgumentType_ItemAmountList);
    MMLCOMMAND_PTR command;
    ASSERT_NO_THROW(
        command = parser.Parse(
            std::string(
                "ADD-RULE:"
                "PRODUCT=[(\"product item name\"|\"product type name\")*1],"
                "RAW_MATERIAL=[(\"raw material A item name\"|\"raw material A type name\")*2 + (\"raw material B item name\"|\"raw material B type name\")*3];"
            )
        )
    );
    ASSERT_STREQ("ADD-RULE", command->name().c_str());
    ItemAmountList product = command->arguments().GetItemAmountList(std::string("PRODUCT"));
    ASSERT_EQ(1, product.size());
    ASSERT_STREQ("product item name", product.begin()->name_pair().name().c_str());
    ASSERT_STREQ("product type name", product.begin()->name_pair().type_name().c_str());
    ASSERT_EQ(1, product.begin()->count());

    ItemAmountList raw_materials = command->arguments().GetItemAmountList(std::string("RAW_MATERIAL"));
    ASSERT_EQ(2, raw_materials.size());
    ItemAmountList::const_iterator ite = raw_materials.begin();
    ASSERT_STREQ("raw material A item name", ite->name_pair().name().c_str());
    ASSERT_STREQ("raw material A type name", ite->name_pair().type_name().c_str());
    ASSERT_EQ(2, ite->count());
    ++ite;
    ASSERT_STREQ("raw material B item name", ite->name_pair().name().c_str());
    ASSERT_STREQ("raw material B type name", ite->name_pair().type_name().c_str());
    ASSERT_EQ(3, ite->count());
    ++ite;
    ASSERT_TRUE(raw_materials.end() == ite);
}

#endif
