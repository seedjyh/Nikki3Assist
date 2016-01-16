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
#include "IFOperator/CodeTransformer.h"

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
        sqlite_file_path_ = CodeTransformer::TransStringToTString(option_map["database"].as<std::string>());
    }
    else
    {
        sqlite_file_path_.assign(_T("mydata.sqlite"));
    }

    if (option_map.count("execute"))
    {
        action_type_ = eProgramActionType_ExecuteMMLCommand;
        mml_to_execute_ = option_map["execute"].as<std::string>();
        return;
    }

    if (option_map.count("source"))
    {
        action_type_ = eProgramActionType_ExecuteMMLScriptFile;
        mml_file_to_execute_ = CodeTransformer::TransStringToTString(option_map["source"].as<std::string>());
        return;
    }

    if (option_map.count("translate"))
    {
        action_type_ = eProgramActionType_TranslateJavaScriptToMMLScript;
        javascript_file_ = CodeTransformer::TransStringToTString(option_map["translate"].as<std::string>());
        return;
    }

    action_type_ = eProgramActionType_CommandLineInterface;

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
        ("version,v", "print version of Nikki3Assist")
        ("help,h", "print this message")
        ("database,d", boost::program_options::value<std::string>(), "sqlite database file path")
        ("execute,e", boost::program_options::value<std::string>(), "execute an MML")
        ("source,s", boost::program_options::value<std::string>(), "execute a MML script file")
        ("translate,t", boost::program_options::value<std::string>(), "translate wardrobe.js to MML script")
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
