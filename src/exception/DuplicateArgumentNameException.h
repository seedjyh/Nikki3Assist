//
//   File name      : DuplicateArgumentNameException.h
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-14 11:02:10
//

#ifndef _DUPLICATEARGUMENTNAMEEXCEPTION_H_
#define _DUPLICATEARGUMENTNAMEEXCEPTION_H_ 1

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

class DuplicateArgumentNameException: public IFException
{
public:
    DuplicateArgumentNameException(const std::string &kName)
    :   IFException(std::string("Duplicate argument name \"") + kName + "\" was detected"){}
    virtual ~DuplicateArgumentNameException(){}
protected:
private:
};

#endif
