//
//   File name      : DirectoryHolder.h
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-11 14:48:28
//

#ifndef _DIRECTORYHOLDER_H_
#define _DIRECTORYHOLDER_H_ 1

// Inherit header
// ...

// C system headers
// ...

// C++ system headers
// ...

// Headers from other projects
// ...

// Headers of current project
#include "IFOperator/PathHandler.h"

class DirectoryHolder
{
    Tstring kPath_;
public:
    DirectoryHolder(const Tstring &kPath)
        : kPath_(kPath)
    {
        if (!PathHandler::CreatePathRecursive(kPath_))
        {
            throw - 1;
        }
    }
    ~DirectoryHolder()
    {
        PathHandler::Remove(kPath_);
    }
protected:
private:
};

#endif
