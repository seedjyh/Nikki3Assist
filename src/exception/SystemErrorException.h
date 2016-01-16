//
//   File name      : SystemErrorException.h
//
//   Code by        : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-16 18:45:44
//

#ifndef _SYSTEMERROREXCEPTION_H_
#define _SYSTEMERROREXCEPTION_H_ 1

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

class SystemErrorException: public IFException
{
public:
    SystemErrorException(const std::string &kModule, const std::string &kDetails)
    :   IFException(std::string("System error! Module=") + kModule + ", Details=" + kDetails){}
    virtual ~SystemErrorException(){}
protected:
private:
};

#endif
