//
//   File name      : UnknownMMLTypeException.h
//
//   Code by        : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-12 21:47:34
//

#ifndef _UNKNOWNMMLTYPEEXCEPTION_H_
#define _UNKNOWNMMLTYPEEXCEPTION_H_ 1

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

class UnknownMMLTypeException: public IFException
{
public:
    UnknownMMLTypeException(const std::string &kType)
    :   IFException(std::string("Detected unknown MML command type: ") + kType){}
    virtual ~UnknownMMLTypeException(){}
protected:
private:
};

#endif
