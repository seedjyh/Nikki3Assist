//
//   File name      : TaskToDo.h
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-18 09:40:48
//

#ifndef _TASKTODO_H_
#define _TASKTODO_H_ 1

// Inherited header
// ...

// C system headers
// ...

// C++ system headers
#include <string>
#include <list>
#include <iostream>

// Headers from other projects
// ...

// Headers of current project
#include "TaskName.h"
#include "ItemAmount.h"

class TaskToDo
{
    TaskName task_name_;
    ItemAmountList targets_;
public:
    TaskToDo(
        const TaskName &kTaskName,
        const ItemAmountList &kTargets
    ):  task_name_(kTaskName),
        targets_(kTargets){}
    TaskToDo(
        const TaskName &kTaskName,
        const ItemAmount &kTarget
    ):  task_name_(kTaskName)
    {
        targets_.push_back(kTarget);
    }
    ~TaskToDo(){}

    friend std::ostream& operator<<(std::ostream &output, const TaskToDo &kThis)
    {
        output << kThis.task_name_ << " to get:" << std::endl;
        for (ItemAmountList::const_iterator ite = kThis.targets_.begin(); ite != kThis.targets_.end(); ++ite)
        {
            output << *ite << std::endl;
        }
        return output;
    }
protected:
private:
};

typedef std::list<TaskToDo> TaskToDoList;

#endif
