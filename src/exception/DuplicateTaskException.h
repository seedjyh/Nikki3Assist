//
//   File name      : DuplicateTaskException.h
//
//   Code by        : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-17 23:19:48
//

#ifndef _DUPLICATETASKEXCEPTION_H_
#define _DUPLICATETASKEXCEPTION_H_ 1

// Inherited header
#include "DatabaseInconsistencyException.h"

// C system headers
// ...

// C++ system headers
// ...

// Headers from other projects
// ...

// Headers of current project
#include "TaskName.h"

class DuplicateTaskException: public DatabaseInconsistencyException
{
public:
    DuplicateTaskException(const TaskName &kTaskName)
    :   DatabaseInconsistencyException(std::string("Duplicate task: ") + kTaskName.str()){}
    virtual ~DuplicateTaskException(){}
protected:
private:
};

#endif
