//
//   File name      : MMLCommand.h
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-12 14:08:43
//

#ifndef _MMLCOMMAND_H_
#define _MMLCOMMAND_H_ 1

// Inherite header
// ...

// C system headers
// ...

// C++ system headers
#include <string>
#include <map>
#include <boost/shared_ptr.hpp>

// Headers from other projects
// ...

// Headers of current project
// ...

typedef std::string MMLArgumentName;
typedef std::string MMLArgumentValue;
typedef std::map<MMLArgumentName, MMLArgumentValue> MMLArguments;

// Standard MML format:
// Command-Type:;
// Command-Type:,,,;
// Command-Type: a = 1, b = 2, c = \"333\", d= \"abc\" ;

class MMLCommand
{
    const std::string kType_;
    const MMLArguments kArguments_;
public:
    MMLCommand(const std::string &kType, const MMLArguments &kArguments)
    :   kType_(kType),
        kArguments_(kArguments){}
    ~MMLCommand(){}

    const std::string& type() const { return kType_; }
    const MMLArguments& arguments() const { return kArguments_; }
    const std::string& GetArgument(const std::string &kName);
protected:
private:
};

typedef boost::shared_ptr<MMLCommand> MMLCOMMAND_PTR;

#endif
