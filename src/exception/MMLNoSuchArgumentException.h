//
//   File name      : MMLNoSuchArgumentException.h
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-12 14:22:54
//

#ifndef _MMLNOSUCHARGUMENTEXCEPTION_H_
#define _MMLNOSUCHARGUMENTEXCEPTION_H_ 1

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

class MMLNoSuchArgumentException:   public IFException
{
public:
    MMLNoSuchArgumentException(const std::string &kArgumentName)
    :   IFException(std::string("No such argument in MML, name=") + kArgumentName){}
    ~MMLNoSuchArgumentException(){}
protected:
private:
};

#endif
