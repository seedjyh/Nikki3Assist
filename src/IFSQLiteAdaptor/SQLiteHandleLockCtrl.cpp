//
//   File name      : SQLiteHandleLockCtrl.cpp
//
//   Code by        : jiangyonghang
//
//   Project name   : IFSQLiteAdaptor
//
//   Create datetime: 2014-02-16 23:31:20
//

// Inherit header
#include "SQLiteHandleLockCtrl.h"

// C system headers
// ...

// C++ system headers
// ...

// Headers from other projects
// ...

// Headers of current project
#include "SQLiteHandle.h"

SQLiteHandleLockCtrl::SQLiteHandleLockCtrl(SQLiteHandle &handle)
:   handle_(handle){
    handle_.Lock();
}

SQLiteHandleLockCtrl::~SQLiteHandleLockCtrl()
{
    handle_.Unlock();
}
