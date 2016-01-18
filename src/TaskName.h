//
//   File name      : TaskName.h
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-18 10:51:45
//

#ifndef _TASKNAME_H_
#define _TASKNAME_H_ 1

// Inherited header
// ...

// C system headers
// ...

// C++ system headers
#include <string>
#include <ostream>

// Headers from other projects
// ...

// Headers of current project
// ...

class TaskName
{
    std::string type_;
    std::string chapter_;
    std::string stage_;
public:
    TaskName(const std::string &kType, const std::string &kChapter, const std::string &kStage)
    :   type_(kType),
        chapter_(kChapter),
        stage_(kStage){}
    ~TaskName(){}

    const std::string& type() const { return type_; }
    const std::string& chapter() const { return chapter_; }
    const std::string& stage() const { return stage_; }

    std::string str() const
    {
        return type_ + ":" + chapter_ + "-" + stage_;
    }

    friend std::ostream& operator<<(std::ostream &output, const TaskName &kThis)
    {
        output << kThis.str();
        return output;
    }
protected:
private:
};

#endif
