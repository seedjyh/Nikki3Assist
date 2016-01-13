//
//   File name      : SQLiteHandleFactory.h
//
//   Code by        : jiangyonghang
//
//   Project name   : IFSQLiteAdaptor
//
//   Create datetime: 2013-02-16 14:05:05
//

#ifndef _IFSQLiteAdaptor_include_SQLiteHandleFactory_h_
#define _IFSQLiteAdaptor_include_SQLiteHandleFactory_h_

// Inherit header
// ...

// C system headers
#include <stdio.h>

// C++ system headers
// ...

// Headers from other projects
// ...

// Headers of current project
#include "SQLiteHandle.h"

class SQLiteLogger;

class SQLiteHandleFactory
{
public:
    static SQLITEHANDLE_PTR Create(const SQLiteLogger *kLogger = NULL);
protected:
private:
    SQLiteHandleFactory(){};
    ~SQLiteHandleFactory(){};
};

#endif

