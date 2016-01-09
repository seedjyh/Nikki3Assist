//
//   File name      : ProgramArguments.cpp
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : IFComicDownloader
//
//   Create datetime: 2015-12-18 15:41:10
//

// Implemented header
#include "ProgramArguments.h"

// C system headers
#include <windows.h>

// C++ system headers
#include <sstream>

// Headers from other projects
// ...

// Headers of current project
#include "IFMacros/BasicOperateMacro.h"
#include "IFDataType/Tstring.h"
#include "IFDataType/IFException.h"

ProgramArguments* ProgramArguments::instance_ = NULL;

ProgramArguments& ProgramArguments::Instance()
{
    if (NULL == instance_)
    {
        instance_ = new ProgramArguments();
    }
    return *instance_;
}

void ProgramArguments::Initialize(int argc, const char * const *argv)
{
    Tstring current_path = GetCurrentPath();

    boost::program_options::variables_map option_map;

    try
    {
        store(parse_command_line(argc, argv, option_desc_), option_map);
    }
    catch (std::exception &e)
    {
        throw IFException(std::string(e.what() ));
    }
    notify(option_map);

    if (option_map.count("help"))
    {
        action_type_ = eProgramActionType_ShowHelp;
        return;
    }

    if (option_map.count("version"))
    {
        action_type_ = eProgramActionType_ShowVersion;
        return;
    }

    if (option_map.count("database"))
    {
        sqlite_file_path_ = option_map["database"].as<std::string>();
    }
    else
    {
        sqlite_file_path_.assign("mydata.sqlite");
    }

    if (option_map.count("import"))
    {
        action_type_ = eProgramActionType_ImportData;
        source_xml_file_path_ = option_map["import"].as<std::string>();
        return;
    }

    if (option_map.count("export"))
    {
        action_type_ = eProgramActionType_ExportData;
        destination_xml_file_path_ = option_map["export"].as<std::string>();
        return;
    }

    if (option_map.count("query"))
    {
        action_type_ = eProgramActionType_QueryClotheInfo;
        queried_item_id_ = option_map["query"].as<int>();
        return;
    }

    if (option_map.count("acquire"))
    {
        action_type_ = eProgramActionType_ShowClotheAcquisitionMean;
        queried_item_id_ = option_map["acquire"].as<int>();
        return;
    }

    return;
}

std::string ProgramArguments::GetArgumentHelp() const
{
    std::stringstream ss;
    ss << option_desc_;
    return ss.str();
}

ProgramArguments::ProgramArguments()
:   option_desc_("command line options")
{
    option_desc_.add_options()
        ("help,h", "print this message")
        ("version,v", "print version of VXI")
        ("database,d", boost::program_options::value<std::string>(), "sqlite database file path")
        ("import,i", boost::program_options::value<std::string>(), "import data from file to database")
        ("export,e", boost::program_options::value<std::string>(), "export data from database to file")
        ("query,q", boost::program_options::value<int>(), "query information of item")
        ("acquire,a", boost::program_options::value<int>(), "show acquisition mean of item")
        ;
}

Tstring ProgramArguments::GetCurrentPath()
{
    TCHAR buffer[MAX_PATH];
    if (0 == GetCurrentDirectory(MY_SIZE_OF_ARRAY(buffer), buffer))
    {
        throw std::string("Get current path failed.");
    }

    _tcscat(buffer, _T("\\"));
    return Tstring(buffer);
}
