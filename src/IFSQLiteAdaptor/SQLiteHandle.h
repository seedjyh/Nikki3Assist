//
//   File name      : SQLiteHandle.h
//
//   Code by        : jiangyonghang
//
//   Project name   : IFSQLiteAdaptor
//
//   Create datetime: 2013-02-16 13:56:28
//

#ifndef _IFSQLiteAdaptor_include_SQLiteHandle_h_
#define _IFSQLiteAdaptor_include_SQLiteHandle_h_

// Inherit header
// ...

// C system headers
// ...

// C++ system headers
#include <string>
#include <boost/shared_ptr.hpp>

// Headers from other projects
// ...

// Headers of current project
#include "SQLExecuteResult.h"

class SQLiteHandle
{
public:
    SQLiteHandle(){};
    virtual ~SQLiteHandle(){};

    virtual int Open(const std::string &kDatabaseName) = 0;

    virtual void Close() = 0;

    virtual SQLEXECUTERESULT_PTR ExecuteSQL(const std::string &kSQL) = 0;

    virtual void Lock() = 0;

    virtual void Unlock() = 0;

protected:
private:
};

typedef boost::shared_ptr<SQLiteHandle> SQLITEHANDLE_PTR;

#endif

