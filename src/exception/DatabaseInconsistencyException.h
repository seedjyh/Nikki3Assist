//
//   File name      : DatabaseInconsistencyException.h
//
//   Code by        : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-12 23:49:33
//

#ifndef _DATABASEINCONSISTENCYEXCEPTION_H_
#define _DATABASEINCONSISTENCYEXCEPTION_H_ 1

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

class DatabaseInconsistencyException: public IFException
{
public:
    DatabaseInconsistencyException(const std::string &kDetails)
    :   IFException(std::string("Detected inconsistency in database") + kDetails){}
protected:
private:
};

#endif
