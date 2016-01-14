//
//   File name      : System.cpp
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-11 14:04:34
//

// Implemented header
#include "System.h"

// C system headers
// ...

// C++ system headers
#include <iostream>

// Headers from other projects
// ...

// Headers of current project
#include "ProgramArguments.h"
#include "GlobalEnum.h"
#include "DatabaseOperator.h"
#include "DataHolderFileOperator.h"
#include "IFDataType/IFException.h"
#include "IFOperator/CodeTransformer.h"
#include "MMLExecutor.h"
#include "MMLCommandFactory.h"

static const std::string s_kVersion("0.0.0.2");

void System::Run()
{
    if (eProgramActionType_ShowVersion == ProgramArguments::Instance().action_type())
    {
        std::cout << "Nikki3Assist (" << s_kVersion << ")" << std::endl;
        std::cout << "Build time: " << __TIME__ << ", " << __DATE__ << std::endl;
    }
    else if (eProgramActionType_ShowHelp == ProgramArguments::Instance().action_type())
    {
        std::cout << ProgramArguments::Instance().GetArgumentHelp() << std::endl;
    }
    else if (eProgramActionType_ExecuteMMLCommand == ProgramArguments::Instance().action_type())
    {
        MMLExecutor mml_executor(DatabaseOperator(ProgramArguments::Instance().sqlite_file_path()));
        mml_executor.ExecuteSingleCommand(*MMLCommandFactory::Create(ProgramArguments::Instance().mml_to_execute()));
        std::cout << "Execute MML: " << ProgramArguments::Instance().mml_to_execute() << std::endl;
    }
    else if (eProgramActionType_ExecuteMMLScriptFile == ProgramArguments::Instance().action_type())
    {
        MMLList mml_list = MMLCommandFactory::ParseScriptFile(ProgramArguments::Instance().mml_file_to_execute());
        MMLExecutor mml_executor(DatabaseOperator(ProgramArguments::Instance().sqlite_file_path()));
        for (MMLList::const_iterator ite = mml_list.begin(); ite != mml_list.end(); ++ite)
        {
            mml_executor.ExecuteSingleCommand(**ite);
        }
        std::cout << "Execute MML file: " <<  CodeTransformer::TransTstringToString(ProgramArguments::Instance().mml_file_to_execute()) << std::endl;
    }
    else
    {
        throw IFException(std::string("Unknown argument list"));
    }

    return;
}
