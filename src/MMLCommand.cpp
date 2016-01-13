//
//   File name      : MMLCommand.cpp
//
//   Code by        : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-12 22:43:12
//

// Inherited header
#include "MMLCommand.h"

// C system headers
// ...

// C++ system headers
// ...

// Headers from other projects
// ...

// Headers of current project
#include "exception/MMLNoSuchArgumentException.h"

const std::string& MMLCommand::GetArgument(const std::string &kName)
{
    MMLArguments::const_iterator ite = kArguments_.find(kName);
    if (kArguments_.end() == ite)
    {
        throw MMLNoSuchArgumentException(kName);
    }
    return ite->second;
}
