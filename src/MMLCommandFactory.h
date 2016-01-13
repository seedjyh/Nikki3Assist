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
#include "MMLCommand.h"

typedef std::list<MMLCOMMAND_PTR> MMLList;
class Tstring;

class MMLCommandFactory
{
public:
    // Parse the text and return MML command.
    // Throw MMLFormatErrorException if something wrong.
    static MMLCOMMAND_PTR Create(const std::string &kCommandText);
    static MMLList ParseScriptFile(const Tstring &kScriptFilePath);

protected:
private:
    MMLCommandFactory(){}
    ~MMLCommandFactory(){}
    static void ParseMMLCommandText(const std::string &kText, std::string &ret_type, MMLArguments &ret_arguments);
    static int JumpBlankCharacter(const std::string &kText, int now_index);
    static std::string ReadLetterSeries(const char kText[]);
    static std::string ReadQuotation(const char kText[]);
    static std::string ReadNumber(const char kText[]);
};

#endif
