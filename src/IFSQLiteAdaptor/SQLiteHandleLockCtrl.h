//
//   File name      : SQLiteHandleLockCtrl.h
//
//   Code by        : jiangyonghang
//
//   Project name   : IFSQLiteAdaptor
//
//   Create datetime: 2014-02-16 23:30:30
//

#ifndef _IFSQLITEADAPTOR_INCLUDE_SQLITEHANDLELOCKCTRL_H_
#define _IFSQLITEADAPTOR_INCLUDE_SQLITEHANDLELOCKCTRL_H_ 1

// Inherit header
// ...

// C system headers
// ...

// C++ system headers
// ...

// Headers from other projects
// ...

// Headers of current project
// ...

// This class is prepared to safely lock and unlock the entire sqlite handle

class SQLiteHandle;
class SQLiteHandleLockCtrl
{
    SQLiteHandle &handle_;
public:
    SQLiteHandleLockCtrl(SQLiteHandle &handle);
    ~SQLiteHandleLockCtrl();
protected:
private:
};

#endif

