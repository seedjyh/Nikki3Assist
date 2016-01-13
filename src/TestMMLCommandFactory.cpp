//
//   File name      : TestMMLCommandFactory.cpp
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-12 16:47:29
//

#ifdef _GTEST_

// Implemented header
#include "MMLCommandFactory.h"

// C system headers
// ...

// C++ system headers
// ...

// Headers from other projects
#include <gtest/gtest.h>

// Headers of current project
#include "exception/MMLFormatErrorException.h"
#include "exception/MMLNoSuchArgumentException.h"
#include "IFDataType/Tstring.h"
#include "DirectoryHolder.h"
#include "IFDataType/DataHolder.h"
#include "DataHolderFileOperator.h"

TEST(MMLCommandFactory, Create_Error)
{
    ASSERT_THROW(MMLCommandFactory::Create(std::string("")), MMLFormatErrorException);
    ASSERT_THROW(MMLCommandFactory::Create(std::string(";")), MMLFormatErrorException); // no type mark (':')
    ASSERT_THROW(MMLCommandFactory::Create(std::string("b=1,c=2;")), MMLFormatErrorException); // no type part
    ASSERT_THROW(MMLCommandFactory::Create(std::string("a:b=1,c=2")), MMLFormatErrorException); // no ';' as ending
    ASSERT_THROW(MMLCommandFactory::Create(std::string("a:b=1;c=2;")), MMLFormatErrorException); // found ';' in the middle of the command
    ASSERT_THROW(MMLCommandFactory::Create(std::string("a:b=1,b=2;")), MMLFormatErrorException); // duplicate argument
    ASSERT_THROW(MMLCommandFactory::Create(std::string("a:a=1,b=x;")), MMLFormatErrorException); // argument value error: b=x should be b="x"
}

TEST(MMLCommandFactory, Create_OK)
{
    MMLCOMMAND_PTR mml;
    ASSERT_NO_THROW(mml = MMLCommandFactory::Create(std::string("command:;")));
    ASSERT_STREQ("command", mml->type().c_str());
    ASSERT_TRUE(mml->arguments().empty());

    ASSERT_NO_THROW(mml = MMLCommandFactory::Create(std::string("command:a=1,b=2,c=\"xyz\";")));
    ASSERT_STREQ("command", mml->type().c_str());
    ASSERT_STREQ("1", mml->arguments().find(std::string("a"))->second.c_str());
    ASSERT_STREQ("2", mml->arguments().find(std::string("b"))->second.c_str());
    ASSERT_STREQ("xyz", mml->arguments().find(std::string("c"))->second.c_str());
}

TEST(MMLCommandFactory, ParseScriptFile)
{
    const Tstring kTestDirectoryPath(_T("test\\"));
    DirectoryHolder dir_holder(kTestDirectoryPath);
    const Tstring kScriptFilePath = kTestDirectoryPath + _T("mml-script.txt");
    DATAHOLDER_PTR script_text(new DataHolder());

    script_text->Append(std::string(" \t\n  \n"));
    script_text->Append(std::string("command-A:;"));
    script_text->Append(std::string(" \n\t\n  \n"));
    script_text->Append(std::string("command-B:a=1,b=\"hello\";"));
    script_text->Append(std::string(" \n\t\r\n  \t  \n"));
    script_text->Append(std::string("command-C:,x=99,;"));
    script_text->Append(std::string("\n  \t  \n"));

    DataHolderFileOperator::WriteToFile(script_text, kScriptFilePath);

    MMLList mml_list;
    ASSERT_NO_THROW(mml_list = MMLCommandFactory::ParseScriptFile(kScriptFilePath));

    ASSERT_EQ(3, mml_list.size());
    MMLList::const_iterator ite = mml_list.begin();
    ASSERT_STREQ("command-A", (*ite)->type().c_str());
}

#endif
