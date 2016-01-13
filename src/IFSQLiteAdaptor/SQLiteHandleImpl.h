//
//   File name      : SQLiteHandleImpl.h
//
//   Code by        : jiangyonghang
//
//   Project name   : IFSQLiteAdaptor
//
//   Create datetime: 2013-02-16 16:44:06
//

#ifndef _IFSQLiteAdaptor_src_SQLiteHandleImpl_h_
#define _IFSQLiteAdaptor_src_SQLiteHandleImpl_h_

// Inherit header
#include "SQLiteHandle.h"

// C system headers
// ...

// C++ system headers
// ...

// Headers from other projects
#include "IFDataType/Locker.h"

// Headers of current project
#include "sqlitesrc/sqlite3.h"
#include "SQLResultDataStructure.h"

class SQLiteLogger;

class SQLiteHandleImpl: public SQLiteHandle
{
    sqlite3 *db_handle_;
    const SQLiteLogger *kLogger_;
    Locker mutex_;
public:
    SQLiteHandleImpl(const SQLiteLogger *kLogger = NULL);
    virtual ~SQLiteHandleImpl();

    virtual int Open(const std::string &kDatabaseName);

    virtual void Close();

    virtual SQLEXECUTERESULT_PTR ExecuteSQL(const std::string &kSQL);

    virtual void Lock();

    virtual void Unlock();

protected:
private:
    static int SaveOneLine(sqlite3_stmt *statement_handle, SqlResultLine &ret_line);
};

#endif

