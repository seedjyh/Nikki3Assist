//
//   File name      : TestDataHolderFileOperator.cpp
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-11 14:46:38
//

#ifdef _GTEST_

// Implemented header
#include "DataHolderFileOperator.h"

// C system headers
#include <tchar.h>

// C++ system headers
// ...

// Headers from other projects
#include <gtest/gtest.h>

// Headers of current project
#include "DirectoryHolder.h"

TEST(DataHolderFileOperator, WriteToFile_CreateFromFile)
{
    Tstring test_dir_path(_T("test\\"));
    Tstring file_path = test_dir_path + _T("hello.dat");
    DirectoryHolder dir_holder(test_dir_path);
    std::string kOriginalData("Hello, world");
    DataHolderFileOperator::WriteToFile(DATAHOLDER_PTR(new DataHolder(kOriginalData)), file_path);
    DATAHOLDER_PTR new_data = DataHolderFileOperator::CreateFromFile(file_path);
    ASSERT_EQ(kOriginalData.size(), new_data->size());
    ASSERT_EQ(0, memcmp(kOriginalData.c_str(), new_data->content(), kOriginalData.size()));
}

#endif
