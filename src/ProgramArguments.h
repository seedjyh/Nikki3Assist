//
//   File name      : ProgramArguments.h
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : IFComicDownloader
//
//   Create datetime: 2015-12-18 14:39:31
//

#ifndef _PROGRAMARGUMENTS_H_
#define _PROGRAMARGUMENTS_H_ 1

// Inherite header
// ...

// C system headers
// ...

// C++ system headers
#include <boost/program_options.hpp>
#include <string>

// Headers from other projects
// ...

// Headers of current project
#include "GlobalEnum.h"
#include "GlobalDataType.h"
#include "IFDataType/Tstring.h"

// Use Singleton pattern.

class ProgramArguments
{
    static ProgramArguments *instance_;

    boost::program_options::options_description option_desc_;

    eProgramActionType action_type_;
    Tstring sqlite_file_path_;
    std::string mml_to_execute_;
    Tstring mml_file_to_execute_;
    Tstring javascript_file_;
public:
    static ProgramArguments& Instance();

    void Initialize(int argc, const char * const *argv);
    std::string GetArgumentHelp() const;

    eProgramActionType action_type() const { return action_type_; }
    const Tstring& sqlite_file_path() const { return sqlite_file_path_; }
    const std::string& mml_to_execute() const { return mml_to_execute_; }
    const Tstring& mml_file_to_execute() const { return mml_file_to_execute_; }
protected:
private:
    ProgramArguments();
    ~ProgramArguments(){}
    static Tstring GetCurrentPath();
};

#endif
