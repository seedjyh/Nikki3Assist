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

MMLCOMMAND_PTR MMLCommandFactory::ParseSingleCommand(const std::string &kText)
{
    for (std::list<MMLParser>::const_iterator ite = parser_list_.begin(); ite != parser_list_.end(); ++ite)
    {
        if (ite->CheckCommandName(kText))
        {
            int read_count = 0;
            MMLCOMMAND_PTR result = ite->Parse(kText, read_count);
            if (read_count != kText.size())
            {
                throw MMLFormatErrorException(kText, std::string("Detected extra data after command"));
            }
            return result;
        }
    }
    throw MMLFormatErrorException(kText, std::string("Undefined MML"));
}

MMLList MMLCommandFactory::ParseScriptFile(const Tstring &kScriptFilePath)
{
    DATAHOLDER_PTR script_data = DataHolderFileOperator::CreateFromFile(kScriptFilePath);
    MMLList result;
    if (script_data->size() > 0)
    {
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
            result.push_back(ParseSingleCommand(std::string(p, command_length)));
            p += command_length;
        }
    }
    return result;
}

void MMLCommandFactory::InitializeParsers()
{
    parser_list_.clear();
    parser_list_.push_back(MMLParser(std::string("ADD-ITEM-TYPE"))
        (std::string("TYPE"), MMLParser::eArgumentType_String)
    );
    parser_list_.push_back(MMLParser(std::string("ADD-ITEM-INFO"))
        (std::string("NAME"), MMLParser::eArgumentType_ItemNamePair)
        (std::string("ID_IN_GAME"), MMLParser::eArgumentType_Integer)
    );
    parser_list_.push_back(MMLParser(std::string("SET-ITEM-PRICE"))
        (std::string("ITEM"), MMLParser::eArgumentType_ItemNamePair)
        (std::string("PRICE"), MMLParser::eArgumentType_ItemAmountList)
    );
    parser_list_.push_back(MMLParser(std::string("SET-ITEM-STOCK"))
        (std::string("ITEM"), MMLParser::eArgumentType_ItemNamePair)
        (std::string("COUNT"), MMLParser::eArgumentType_Integer)
    );
    parser_list_.push_back(MMLParser(std::string("SET-CREATING-RULE"))
        (std::string("PRODUCT"), MMLParser::eArgumentType_ItemNamePair)
        (std::string("RAW_MATERIALS"), MMLParser::eArgumentType_ItemAmountList)
    );
    parser_list_.push_back(MMLParser(std::string("SET-DYEING-RULE"))
        (std::string("PRODUCT"), MMLParser::eArgumentType_ItemNamePair)
        (std::string("RAW_MATERIALS"), MMLParser::eArgumentType_ItemAmountList)
    );
    parser_list_.push_back(MMLParser(std::string("ADD-TASK-INFO"))
        (std::string("TYPE"), MMLParser::eArgumentType_String)
        (std::string("CHAPTER"), MMLParser::eArgumentType_String)
        (std::string("STAGE"), MMLParser::eArgumentType_String)
        (std::string("REWARDS"), MMLParser::eArgumentType_ItemAmountList)
    );
    parser_list_.push_back(MMLParser(std::string("SHOW-ITEM-ACQUISITION-MEAN"))
        (std::string("TARGET"), MMLParser::eArgumentType_ItemAmountList)
    );

    return;
}
