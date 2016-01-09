//
//   File name      : MainGoogleTest.cpp
//
//   Code by        : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-09 21:03:08
//

#ifdef _GTEST_

// Tested or implemented header
// ...

// C system headers
// ...

// C++ system headers
// ...

// Headers from other projects
#include <gtest/gtest.h>

// Headers of current project
// ...

int main(int arg_count, char **arg_list)
{
    PauseWhenDestruct pauser;
    testing::InitGoogleTest(&arg_count, arg_list);
    return RUN_ALL_TESTS();
}

#endif
