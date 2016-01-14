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

    void ExecuteSingleCommand(const MMLCommand &kCommand);
protected:
private:
    static void RegisterMMLProcessors();
    static MMLProcessor AddItemInfo;
};

#endif
