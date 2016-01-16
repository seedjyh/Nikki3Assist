//
//   File name      : DuplicateItemTypeException.h
//
//   Code by        : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-16 00:37:14
//

#ifndef _DUPLICATEITEMTYPEEXCEPTION_H_
#define _DUPLICATEITEMTYPEEXCEPTION_H_ 1

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

class DuplicateItemTypeException: public IFException
{
public:
    DuplicateItemTypeException(const std::string &kTypeName)
    :   IFException(std::string("Detected duplicate item type: \"") + kTypeName + "\""){}
    virtual ~DuplicateItemTypeException(){}
protected:
private:
};

#endif
