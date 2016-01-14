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
#include "MMLArgumentSet.h"

class MMLCommand
{
    std::string name_;
    MMLArgumentSet arguments_;
public:
    MMLCommand(const std::string &kName, const MMLArgumentSet &kArguments)
    :   name_(kName),
        arguments_(kArguments){}
    ~MMLCommand(){}

    const std::string& name() const { return name_; }
    const MMLArgumentSet& arguments() const { return arguments_; }
protected:
private:
};

typedef boost::shared_ptr<MMLCommand> MMLCOMMAND_PTR;

#endif
