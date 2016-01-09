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
    std::string sqlite_file_path_;
    std::string source_xml_file_path_;
    std::string destination_xml_file_path_;
    int queried_item_id_;
public:
    static ProgramArguments& Instance();

    void Initialize(int argc, const char * const *argv);
    std::string GetArgumentHelp() const;

    eProgramActionType action_type() const { return action_type_; }
    std::string sqlite_file_path() const { return sqlite_file_path_; }
    std::string source_xml_file_path() const { return source_xml_file_path_; }
    std::string destination_xml_file_path() const { return destination_xml_file_path_; }
    ItemUID queried_item_id() const { return queried_item_id_; }
protected:
private:
    ProgramArguments();
    ~ProgramArguments(){}
    static Tstring GetCurrentPath();
};

#endif
