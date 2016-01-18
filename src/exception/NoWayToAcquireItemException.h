//
//   File name      : NoWayToAcquireItemException.h
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-18 15:39:44
//

#ifndef _NOWAYTOACQUIREITEMEXCEPTION_H_
#define _NOWAYTOACQUIREITEMEXCEPTION_H_ 1

// Inherited header
#include "IFDataType/IFException.h"

// C system headers
// ...

// C++ system headers
// ...

// Headers from other projects
// ...

// Headers of current project
#include "ItemNamePair.h"

class NoWayToAcquireItemException: public IFException
{
public:
    NoWayToAcquireItemException(const ItemNamePair &kTarget)
    :   IFException(std::string("No way to acquire ") + kTarget.str()){}
    ~NoWayToAcquireItemException(){}
protected:
private:
};

#endif
