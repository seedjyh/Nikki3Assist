//
//   File name      : MMLCommandFactory.cpp
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-12 16:44:29
//

// Implemented header
#include "MMLCommandFactory.h"

// C system headers
// ...

// C++ system headers
// ...

// Headers from other projects
// ...

// Headers of current project
#include "exception/MMLFormatErrorException.h"
#include "DataHolderFileOperator.h"

MMLCOMMAND_PTR MMLCommandFactory::Create(const std::string &kCommandText)
{
    std::string type;
    MMLArguments arguments;
    ParseMMLCommandText(kCommandText, type, arguments);
    return MMLCOMMAND_PTR(new MMLCommand(type, arguments));
}

void MMLCommandFactory::ParseMMLCommandText(const std::string &kText, std::string &ret_type, MMLArguments &ret_arguments)
{
    ret_type.clear();
    ret_arguments.clear();

    if (kText.empty())
    {
        throw MMLFormatErrorException(kText, "command should not be empty");
    }

    int i = JumpBlankCharacter(kText, 0);
    // remove brank part at the beginning

    if (!isalpha(kText[i]))
    {
        throw MMLFormatErrorException(kText, std::string("command type should start with letter"));
    }

    // type
    for (; i < static_cast<int>(kText.size()); i++)
    {
        if (
            ('-' == kText[i]) ||
            (isalpha(kText[i]))
            )
        {
            ret_type += kText[i];
        }
        else if (':' == kText[i])
        {
            break;
        }
        else
        {
            throw MMLFormatErrorException(kText, std::string("Found unacceptable character in command type part: \'") + kText[i] + "\'");
        }
    }

    if (kText[i] != ':')
    {
        throw MMLFormatErrorException(kText, std::string("Failed to find \':\' in command type part"));
    }
    else
    {
        i++; // jump ':'
    }

    // arguments
    while (1)
    {
        i = JumpBlankCharacter(kText, i);
        if (';' == kText[i])
        {
            break;
        }
        else if (',' == kText[i]) // empty argument
        {
            i++;
            continue;
        }

        std::string argument_name = ReadLetterSeries(kText.c_str() + i);
        if (argument_name.empty())
        {
            throw MMLFormatErrorException(kText, std::string("argument name should not be empty"));
        }
        i += argument_name.size();
        i = JumpBlankCharacter(kText, i);
        if (kText[i] != '=')
        {
            throw MMLFormatErrorException(kText, std::string("failed to find \'=\' after argument name"));
        }
        i++; // Jump '='
        i = JumpBlankCharacter(kText, i);
        std::string argument_value;
        if ('\"' == kText[i])
        {
            argument_value = ReadQuotation(kText.c_str() + i);
            assert('\"' == argument_value[0]);
            assert('\"' == argument_value[argument_value.size() - 1]);
            i += argument_value.size();
            argument_value = std::string(argument_value.c_str() + 1, argument_value.size() - 2 * sizeof('\"'));
        }
        else
        {
            argument_value = ReadNumber(kText.c_str() + i);
            i += argument_value.size();
        }
        i = JumpBlankCharacter(kText, i);
        if (
            (',' == kText[i]) ||
            (';' == kText[i])
            )
        {
            if (ret_arguments.find(argument_name) != ret_arguments.end())
            {
                throw MMLFormatErrorException(kText, std::string("Detected duplicate argument name: \"") + argument_name + "\"");
            }
            ret_arguments[argument_name] = argument_value;
        }
        else
        {
            throw MMLFormatErrorException(kText, std::string("reading argument failed"));
        }
    }
    assert(i < static_cast<int>(kText.size()));
    if (kText[i] != ';')
    {
        throw MMLFormatErrorException(kText, std::string("command should end with ';', but not"));
    }

    i++; // Jump ';'
    if (i < static_cast<int>(kText.size()))
    {
        i = JumpBlankCharacter(kText, i);
    }
    if (i != kText.size())
    {
        throw MMLFormatErrorException(kText, std::string("semi-colon appears at the middle of the MML command"));
    }
    return;
}

MMLList MMLCommandFactory::ParseScriptFile(const Tstring &kScriptFilePath)
{
    DATAHOLDER_PTR script_data = DataHolderFileOperator::CreateFromFile(kScriptFilePath);
    MMLList result;
    for (const char *p = script_data->content();;)
    {
        const char *current_command_end = strchr(p, ';');
        if (NULL == current_command_end)
        {
            break;
        }
        if (p >= script_data->content() + script_data->size())
        {
            break;
        }
        int command_length = current_command_end - p + 1; // including the ';'.
        result.push_back(Create(std::string(p, command_length)));
        p += command_length;
    }
    return result;
}

int MMLCommandFactory::JumpBlankCharacter(const std::string &kText, int now_index)
{
    int i = now_index;
    if (i >= static_cast<int>(kText.size()))
    {
        throw MMLFormatErrorException(kText, std::string("out-of-bounds access detected"));
    }
    for (; i < static_cast<int>(kText.size()); i++)
    {
        if (
            (kText[i] != ' ') &&
            (kText[i] != '\t') &&
            (kText[i] != '\r') &&
            (kText[i] != '\n')
            )
        {
            break;
        }
    }
    return i;
}

std::string MMLCommandFactory::ReadLetterSeries(const char kText[])
{
    std::string result;
    while (isalpha(*kText))
    {
        result += *kText;
        kText++;
    }
    return result;
}

std::string MMLCommandFactory::ReadQuotation(const char kText[])
{
    assert('\"' == *kText);

    // eat the open '\"'
    std::string result = "\"";
    kText++;

    // eat content
    while (*kText != NULL)
    {
        result += *kText;
        if ('\"' == *kText) // eaten character is the close '\"'
        {
            break;
        }
        kText++;
    }
    return result;
}

std::string MMLCommandFactory::ReadNumber(const char kText[])
{
    std::string result;
    while (isdigit(*kText))
    {
        result += *kText;
        kText++;
    }
    return result;
}
