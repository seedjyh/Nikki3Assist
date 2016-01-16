//
//   File name      : MMLParser.h
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-14 14:00:34
//

#ifndef _MMLPARSER_H_
#define _MMLPARSER_H_ 1

// Inherited header
// ...

// C system headers
// ...

// C++ system headers
#include <string>

// Headers from other projects
// ...

// Headers of current project
#include "MMLCommand.h"

class MMLParser
{
public:
    enum eArgumentType
    {
        eArgumentType_Integer = 0,
        eArgumentType_String,
        eArgumentType_ItemNamePair,
        eArgumentType_ItemAmountList
    };
    typedef std::map<std::string, eArgumentType> ArgumentNameTypeMap;
private:
    std::string name_;
    ArgumentNameTypeMap argument_definitions_;
public:
    explicit MMLParser(const std::string &kName);
    ~MMLParser(){}

    MMLParser& operator()(const std::string &kArgumentName, const eArgumentType &kArgumentType);

    bool CheckCommandName(const std::string &kText) const;

    // @ret_read_count: Number of bytes this function read. Including the command and blank characters after the semi-colon if they exist.
    MMLCOMMAND_PTR Parse(const std::string &kText, int &ret_read_count) const;
protected:
private:
    std::string ParseCommandName(const char kText[], int &ret_read_count) const;
    std::string ParseArgumentName(const char kText[], int &ret_read_count) const;
    int ParseInteger(const char kText[], int &ret_read_count) const;
    std::string ParseString(const char kText[], int &ret_read_count) const;
    ItemNamePair ParseItemNamePair(const char kText[], int &ret_read_count) const;
    ItemAmountList ParseItemAmountList(const char kText[], int &ret_read_count) const;
    ItemAmount ParseItemAmount(const char kText[], int &ret_read_count) const;
    static int JumpBlankCharacter(const char kText[]);
    static bool CheckCharacter(char current, const char kTargets[]);
};

#endif
