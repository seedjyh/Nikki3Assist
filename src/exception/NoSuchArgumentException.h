//
//   File name      : NoSuchArgumentException.h
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-14 11:12:44
//

#ifndef _NOSUCHARGUMENTEXCEPTION_H_
#define _NOSUCHARGUMENTEXCEPTION_H_ 1

// Inherited header
#include "IFDataType/IFException.h"

// C system headers
// ...

// C++ system headers
// ...

// Headers from other projects
// ...

// Headers of current project
// ...

class NoSuchArgumentException: public IFException
{
public:
    NoSuchArgumentException(const std::string &kName)
    :   IFException(std::string("No such argument: \"") + kName + "\""){};
    virtual ~NoSuchArgumentException(){}
protected:
private:
};

#endif
