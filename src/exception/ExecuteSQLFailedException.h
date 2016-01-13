//
//   File name      : ExecuteSQLFailedException.h
//
//   Code by        : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-12 23:45:58
//

#ifndef _EXECUTESQLFAILEDEXCEPTION_H_
#define _EXECUTESQLFAILEDEXCEPTION_H_ 1

// Inherited header
#include "IFDataType/IFException.h"

// C system headers
// ...

// C++ system headers
#include <sstream>

// Headers from other projects
// ...

// Headers of current project
// ...

class ExecuteSQLFailedException: public IFException
{
public:
    ExecuteSQLFailedException(const std::string &kSQL, int return_code)
    :   IFException(GenerateMessage(kSQL, return_code)){}
    virtual ~ExecuteSQLFailedException(){}
protected:
private:
    static std::string GenerateMessage(const std::string &kSQL, int returned_code)
    {
        std::stringstream ss;
        ss << "Execute SQL returned " << returned_code << ", sql=" << kSQL;
        return ss.str();
    }
};

#endif
