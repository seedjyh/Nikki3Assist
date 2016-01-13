//
//   File name      : SQLiteHandleFactory.cpp
//
//   Code by        : jiangyonghang
//
//   Project name   : IFSQLiteAdaptor
//
//   Create datetime: 2013-02-16 16:33:20
//

// Inherit header
#include "SQLiteHandleFactory.h"

// C system headers
// ...

// C++ system headers
// ...

// Headers from other projects
// ...

// Headers of current project
#include "SQLiteHandleImpl.h"

SQLITEHANDLE_PTR SQLiteHandleFactory::Create(const SQLiteLogger *kLogger /* = NULL */)
{
    return SQLITEHANDLE_PTR(new SQLiteHandleImpl(kLogger));
}

