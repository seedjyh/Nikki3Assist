//
//   File name      : NoSuchItemIDException.h
//
//   Code by        : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-12 22:34:47
//

#ifndef _NOSUCHITEMIDEXCEPTION_H_
#define _NOSUCHITEMIDEXCEPTION_H_ 1

// Inherited header
#include "IFDataType/IFException.h"

// C system headers
// ...

// C++ system headers
#include <sstream>

// Headers from other projects
// ...

// Headers of current project
#include "GlobalDataType.h"

class NoSuchItemIDException: public IFException
{
public:
    NoSuchItemIDException(ItemID id)
    :   IFException(GenerateMessage(id)){}
    virtual ~NoSuchItemIDException(){}
protected:
private:
    static std::string GenerateMessage(ItemID id)
    {
        std::stringstream ss;
        ss << "No such item ID: " << id;
        return ss.str();
    }
};

#endif
