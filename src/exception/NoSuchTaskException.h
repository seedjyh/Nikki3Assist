//
//   File name      : NoSuchTaskException.h
//
//   Code by        : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-17 23:27:43
//

#ifndef _NOSUCHTASKEXCEPTION_H_
#define _NOSUCHTASKEXCEPTION_H_ 1

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

class NoSuchTaskException : public DatabaseInconsistencyException
{
public:
    NoSuchTaskException(const TaskName &kTaskName)
    :   DatabaseInconsistencyException(std::string("No such task: ") + kTaskName.str()){}
    virtual ~NoSuchTaskException(){}
protected:
private:
};

#endif
