//
//   File name      : MMLParser.cpp
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-14 14:04:28
//

// Implemented header
#include "MMLParser.h"

// C system headers
// ...

// C++ system headers
// ...

// Headers from other projects
// ...

// Headers of current project
#include "exception/MMLFormatErrorException.h"

MMLParser::MMLParser(const std::string &kName)
:   name_(kName){}

MMLParser& MMLParser::operator()(const std::string &kArgumentName, const eArgumentType &kArgumentType)
{
    assert(argument_definitions_.end() == argument_definitions_.find(kArgumentName));
    argument_definitions_[kArgumentName] = kArgumentType;
    return *this;
}

bool MMLParser::CheckCommandName(const std::string &kText) const
{
    int read_count = 0;
    std::string name_in_text = ParseCommandName(kText.c_str(), read_count);
    return (name_ == name_in_text);
}

MMLCOMMAND_PTR MMLParser::Parse(const std::string &kText, int &ret_read_count) const
{
    int read_count = 0;
    std::string name;
    MMLArgumentSet arguments;

    int total_read_count = 0;
    name = ParseCommandName(kText.c_str(), read_count);
    total_read_count = read_count;

    std::set<std::string> found_argument_names;
    bool found_end_semicolon = false;
    while (total_read_count < static_cast<int>(kText.size()))
    {
        // read argument name
        std::string argument_name = ParseArgumentName(kText.c_str() + total_read_count, read_count);
        total_read_count += read_count;

        if (found_argument_names.find(argument_name) != found_argument_names.end())
        {
            throw MMLFormatErrorException(kText, std::string("Duplicate argument: \"") + argument_name + "\"");
        }
        found_argument_names.insert(argument_name);

        // read '='
        total_read_count += JumpBlankCharacter(kText.c_str() + total_read_count);
        if (CheckCharacter(kText[total_read_count], "="))
        {
            total_read_count++;
        }
        else
        {
            throw MMLFormatErrorException(kText, std::string("No required equal sign (\'=\') after argument name: \"") + argument_name + "\"");
        }

        // read argument value
        ArgumentNameTypeMap::const_iterator ite = argument_definitions_.find(argument_name);
        if (argument_definitions_.end() == ite)
        {
            throw MMLFormatErrorException(std::string(kText), std::string("Undefined argument name: \"") + argument_name + "\"");
        }
        switch (ite->second)
        {
        case eArgumentType_Integer:
            arguments.AppendInteger(argument_name, ParseInteger(kText.c_str() + total_read_count, read_count));
            total_read_count += read_count;
            break;
        case eArgumentType_String:
            arguments.AppendString(argument_name, ParseString(kText.c_str() + total_read_count, read_count));
            total_read_count += read_count;
            break;
        case eArgumentType_ItemNamePair:
            arguments.AppendItemNamePair(argument_name, ParseItemNamePair(kText.c_str() + total_read_count, read_count));
            total_read_count += read_count;
            break;
        case eArgumentType_ItemAmountList:
            arguments.AppendItemAmountList(argument_name, ParseItemAmountList(kText.c_str() + total_read_count, read_count));
            total_read_count += read_count;
            break;
        default:
            assert(false);
            break;
        }

        total_read_count += JumpBlankCharacter(kText.c_str() + total_read_count);
        if (',' == kText[total_read_count])
        {
            total_read_count++;
        }
        else if (';' == kText[total_read_count])
        {
            found_end_semicolon = true;
            total_read_count++;
            break;
        }
        else
        {
            throw MMLFormatErrorException(kText, std::string("No required \',\' or \';\' after value of argument: \"") + argument_name + "\"");
        }
        total_read_count += JumpBlankCharacter(kText.c_str() + total_read_count);
    }

    if (!found_end_semicolon)
    {
        throw MMLFormatErrorException(kText, std::string("No required \';\' as the end of MML command"));
    }
    ret_read_count = total_read_count;
    return MMLCOMMAND_PTR(new MMLCommand(name, arguments));
}

std::string MMLParser::ParseCommandName(const char kText[], int &ret_read_count) const
{
    std::string result;
    int begin_index = JumpBlankCharacter(kText);
    for (int i = begin_index; '\0' != kText[i]; i++)
    {
        if (isalpha(kText[i]))
        {
            continue;
        }
        else if ('-' == kText[i])
        {
            continue;
        }
        else if (':' == kText[i])
        {
            ret_read_count = i + strlen(":");
            return std::string(kText, begin_index, i - begin_index);
        }
        else
        {
            throw MMLFormatErrorException(std::string(kText), std::string("Invalid character in command name"));
        }
    }
    throw MMLFormatErrorException(std::string(kText), std::string("No end of command name"));
}

std::string MMLParser::ParseArgumentName(const char kText[], int &ret_read_count) const
{
    std::string result;
    int begin_index = JumpBlankCharacter(kText);
    for (int i = begin_index; '\0' != kText[i]; i++)
    {
        if (isalpha(kText[i]))
        {
            continue;
        }
        else if ('_' == kText[i])
        {
            continue;
        }
        else if (
            (' ' == kText[i]) ||
            ('\t' == kText[i]) ||
            ('=' == kText[i])
        )
        {
            ret_read_count = i;
            return std::string(kText, begin_index, i - begin_index);
        }
        else
        {
            throw MMLFormatErrorException(std::string(kText), std::string("Invalid character in argument name"));
        }
    }
    throw MMLFormatErrorException(std::string(kText), std::string("No end of argument name"));
}

int MMLParser::ParseInteger(const char kText[], int &ret_read_count) const
{
    // The first digit should be 1-9. '0' shell not be the first digit.
    if (kText[0] < '1' || kText[0] > '9')
    {
        throw MMLFormatErrorException(kText, std::string("The first character should be 1~9"));
    }
    int result = kText[0] - '0';
    int read_count = 1;

    // read digits following.
    for (int i = 1;; i++)
    {
        if (isdigit(kText[i]))
        {
            result = result * 10 + (kText[i] - '0');
            read_count++;
        }
        else
        {
            break;
        }
    }

    ret_read_count = read_count;
    return result;
}

std::string MMLParser::ParseString(const char kText[], int &ret_read_count) const
{
    int total_read_count = JumpBlankCharacter(kText);
    if (kText[total_read_count] != '\"')
    {
        throw MMLFormatErrorException(kText, std::string("Can not find LEFT quotation mark for string value"));
    }
    total_read_count += strlen("\"");
    const char *kBegin = kText + total_read_count;

    const char *kEnd = strchr(kText + total_read_count, '\"');
    if (NULL == kEnd)
    {
        throw MMLFormatErrorException(kText, std::string("Can not find RIGHT quotation mark for string value"));
    }

    ret_read_count = kEnd - kText + strlen("\"");
    return std::string(kBegin, (kEnd - kBegin));
}

ItemNamePair MMLParser::ParseItemNamePair(const char kText[], int &ret_read_count) const
{
    int total_read_count = JumpBlankCharacter(kText);
    if (kText[total_read_count] != '(')
    {
        throw MMLFormatErrorException(kText, std::string("Can not find LEFT parenthesis for string value"));
    }
    total_read_count += strlen("(");

    int read_count = 0;
    std::string item_name = ParseString(kText + total_read_count, read_count);
    total_read_count += read_count;

    if (kText[total_read_count] != '|')
    {
        throw MMLFormatErrorException(kText, std::string("Can not find LEFT parenthesis for string value"));
    }
    total_read_count += strlen("|");

    read_count = 0;
    std::string type_name = ParseString(kText + total_read_count, read_count);
    total_read_count += read_count;

    total_read_count += JumpBlankCharacter(kText + total_read_count);

    if (kText[total_read_count] != ')')
    {
        throw MMLFormatErrorException(kText, std::string("Can not find RIGHT parenthesis for string value"));
    }
    total_read_count += strlen(")");

    ret_read_count = total_read_count;
    return ItemNamePair(item_name, type_name);
}

ItemAmountList MMLParser::ParseItemAmountList(const char kText[], int &ret_read_count) const
{
    ItemAmountList result;
    int total_read_count = JumpBlankCharacter(kText);
    if (kText[total_read_count] != '[')
    {
        throw MMLFormatErrorException(std::string(kText), std::string("Can not find \'[\' at the beginning of ItemAmountList"));
    }
    total_read_count += strlen("[");

    while (1)
    {
        int read_count = 0;
        result.push_back(ParseItemAmount(kText + total_read_count, read_count));
        total_read_count += read_count;
        total_read_count += JumpBlankCharacter(kText + total_read_count);
        if (']' == kText[total_read_count])
        {
            total_read_count += strlen("]");
            break;
        }
        else if ('+' == kText[total_read_count])
        {
            total_read_count += strlen("]");
            continue;
        }
        else
        {
            throw MMLFormatErrorException(std::string(kText), std::string("Can not find \'+\' or \']\' after an item-amount"));
        }
    }

    ret_read_count = total_read_count;
    return result;
}

ItemAmount MMLParser::ParseItemAmount(const char kText[], int &ret_read_count) const
{
    int total_read_count = JumpBlankCharacter(kText);
    int read_count = 0;
    ItemNamePair item_name = ParseItemNamePair(kText + total_read_count, read_count);
    total_read_count += read_count;

    total_read_count += JumpBlankCharacter(kText + total_read_count);
    if (kText[total_read_count] != '*')
    {
        throw MMLFormatErrorException(std::string(kText), std::string("Can not find star \'*\' between item-pair-name of item-amount argument"));
    }
    total_read_count += strlen("*");
    total_read_count += JumpBlankCharacter(kText + total_read_count);

    int amount = ParseInteger(kText + total_read_count, read_count);
    total_read_count += read_count;

    ret_read_count = total_read_count;
    return ItemAmount(item_name, amount);
}

int MMLParser::JumpBlankCharacter(const char kText[])
{
    const char kBlankCharacters[] = " \r\t\n";
    for (int i = 0; ; i++)
    {
        if (!CheckCharacter(kText[i], kBlankCharacters))
        {
            return i;
        }
    }
}

bool MMLParser::CheckCharacter(char current, const char kTargets[])
{
    if ('\0' == current)
    {
        return false;
    }
    return (strchr(kTargets, current) != NULL);
}
