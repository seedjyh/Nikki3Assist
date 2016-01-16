//
//   File name      : MMLCommandFactory.h
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-12 16:40:06
//

#ifndef _MMLCOMMANDFACTORY_H_
#define _MMLCOMMANDFACTORY_H_ 1

// Inherited header
// ...

// C system headers
// ...

// C++ system headers
#include <list>

// Headers from other projects
// ...

// Headers of current project
#include "MMLParser.h"
#include "MMLCommand.h"

typedef std::list<MMLCOMMAND_PTR> MMLList;
class Tstring;

class MMLCommandFactory
{
    std::list<MMLParser> parser_list_;
public:
    MMLCommandFactory()
    {
        InitializeParsers();
    }
    ~MMLCommandFactory(){}

    // Parse the text and return MML command.
    // Throw MMLFormatErrorException if something wrong.
    MMLCOMMAND_PTR ParseSingleCommand(const std::string &kText);
    MMLList ParseScriptFile(const Tstring &kScriptFilePath);

protected:
private:
    void InitializeParsers();
};

#endif
