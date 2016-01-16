//
//   File name      : MMLExecutor.h
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-12 13:31:51
//

#ifndef _MMLEXECUTOR_H_
#define _MMLEXECUTOR_H_ 1

// Inherite header
// ...

// C system headers
// ...

// C++ system headers
#include <map>

// Headers from other projects
// ...

// Headers of current project
// ...

class DatabaseOperator;
class MMLArgumentSet;
class MMLCommand;

typedef void MMLProcessor(DatabaseOperator &, const MMLArgumentSet &);
typedef std::map<std::string, MMLProcessor*> MMLProcessorMap;

class MMLExecutor
{
    DatabaseOperator &db_;
    static MMLProcessorMap s_processors_;
public:
    MMLExecutor(DatabaseOperator &db)
    :   db_(db)
    {
        if (s_processors_.empty())
        {
            RegisterMMLProcessors();
        }
    }
    ~MMLExecutor(){}

    // Execute MML command by calling DatabaseOperator methods.
    // Need not to check database inconsistency.
    void ExecuteSingleCommand(const MMLCommand &kCommand);
protected:
private:
    static void RegisterMMLProcessors();
    static MMLProcessor AddItemType;
    static MMLProcessor AddItemInfo;
    static MMLProcessor SetItemPrice;
    static MMLProcessor SetItemStock;
    static MMLProcessor AddCreatingRule;
    static MMLProcessor AddDyeingRule;
    static MMLProcessor AddTaskInfo;
    static MMLProcessor ShowTaskInfo;
    static MMLProcessor ShowItemInfo;
    static MMLProcessor ShowItemAcquisitionMean;
};

#endif
