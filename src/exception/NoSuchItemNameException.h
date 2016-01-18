//
//   File name      : NoSuchItemNameException.h
//
//   Code by        : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-12 22:48:40
//

#ifndef _NOSUCHITEMNAMEEXCEPTION_H_
#define _NOSUCHITEMNAMEEXCEPTION_H_ 1

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

class ItemNamePair;

class NoSuchItemNameException: public IFException
{
public:
    NoSuchItemNameException(const ItemNamePair &kItemNamePair)
    :   IFException(std::string("No such item with name=\"") + kItemNamePair.name() + "\" and type-name=\"" + kItemNamePair.type_name() + "\""){}
    virtual ~NoSuchItemNameException(){}
protected:
private:
};

#endif
