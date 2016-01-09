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
#include "GlobalEnum.h"
#include "ProgramArguments.h"

static const std::string s_kVersion("0.0.0.1");

int main(int argc, char **argv)
{
    setlocale(LC_ALL, "chs");
    try
    {
        ProgramArguments::Instance().Initialize(argc, argv);
    }
    catch (IFException &e)
    {
        std::cout << "Caught an exception: " << e.message() << std::endl;
        std::cout << "System is exiting..." << std::endl;
        return EXIT_FAILURE;
    }

    if (eProgramActionType_ShowVersion == ProgramArguments::Instance().action_type())
    {
        std::cout << "Nikki3Assist (" << s_kVersion << ")" << std::endl;
        std::cout << "Build time: " << __TIME__ << ", " << __DATE__ << std::endl;
        return EXIT_SUCCESS;
    }
    else if (eProgramActionType_ShowHelp == ProgramArguments::Instance().action_type())
    {
        std::cout << ProgramArguments::Instance().GetArgumentHelp() << std::endl;
        return EXIT_SUCCESS;
    }
    else if (eProgramActionType_ImportData == ProgramArguments::Instance().action_type())
    {
        std::cout << "Import data: " << ProgramArguments::Instance().sqlite_file_path()
            << " <--- " << ProgramArguments::Instance().source_xml_file_path() << std::endl;
        return EXIT_SUCCESS;
    }
    else if (eProgramActionType_ExportData == ProgramArguments::Instance().action_type())
    {
        std::cout << "Export data: " << ProgramArguments::Instance().sqlite_file_path()
            << " ---> " << ProgramArguments::Instance().destination_xml_file_path() << std::endl;
        return EXIT_SUCCESS;
    }
    else if (eProgramActionType_QueryClotheInfo == ProgramArguments::Instance().action_type())
    {
        std::cout << "Query information of item " << ProgramArguments::Instance().queried_item_id() << std::endl;
        return EXIT_SUCCESS;
    }
    else if (eProgramActionType_ShowClotheAcquisitionMean == ProgramArguments::Instance().action_type())
    {
        std::cout << "Show acquisition mean of item " << ProgramArguments::Instance().queried_item_id() << std::endl;
        return EXIT_SUCCESS;
    }
    else
    {
        std::cout << "Unknown argument list" << std::endl;
        std::cout << ProgramArguments::Instance().GetArgumentHelp() << std::endl;
        return EXIT_SUCCESS;
    }
}

#endif
