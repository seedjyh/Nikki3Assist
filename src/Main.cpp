//
//   File name      : Main.cpp
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : IFComicDownloader
//
//   Create datetime: 2015-12-17 11:14:50
//

#ifndef _GTEST_

// Implemented header
// ...

// C system headers
// ...

// C++ system headers
#include <iostream>

// Headers from other projects
#include "IFDataType/IFException.h"

// Headers of current project
#include "ProgramArguments.h"
#include "System.h"

int main(int argc, char **argv)
{
    setlocale(LC_ALL, "chs");
    try
    {
        ProgramArguments::Instance().Initialize(argc, argv);
        System system;
        system.Run();
    }
    catch (IFException &e)
    {
        std::cout << "Caught an exception: " << e.message() << std::endl;
        std::cout << "System is exiting..." << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

#endif
