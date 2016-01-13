//
//   File name      : SQLiteHandleImpl.cpp
//
//   Code by        : jiangyonghang
//
//   Project name   : IFSQLiteAdaptor
//
//   Create datetime: 2013-02-16 16:48:22
//

// Inherit header
#include "SQLiteHandleImpl.h"

// C system headers
// ...

// C++ system headers
#include <sstream>

// Headers from other projects
#include "IFMacros/BasicProcessMacro.h"

// Headers of current project
#include "SQLExecuteResultImpl.h"
#include "SQLiteLogger.h"

SQLiteHandleImpl::SQLiteHandleImpl(const SQLiteLogger *kLogger /* = NULL */)
:   db_handle_(NULL),
    kLogger_(kLogger){};

SQLiteHandleImpl::~SQLiteHandleImpl()
{
    Close();
}

int SQLiteHandleImpl::Open(const std::string &kDatabaseName)
{
    int nResult  = false;
    int nRetCode = false;

    LockerCtrl locker_ctrl(mutex_);

    if (kLogger_)
    {
        kLogger_->WriteLogForOpen(kDatabaseName);
    }

    MY_PROCESS_ERROR(NULL == db_handle_);

    nRetCode = sqlite3_open(kDatabaseName.c_str(), &db_handle_);
    MY_PROCESS_ERROR(SQLITE_OK == nRetCode);

// Exit1:
    nResult = true;
Exit0:
    return nResult;
}

void SQLiteHandleImpl::Close()
{
    LockerCtrl locker_ctrl(mutex_);

    if (kLogger_)
    {
        kLogger_->WriteLogForClose();
    }

    if (db_handle_)
    {
        sqlite3_close(db_handle_);
        db_handle_ = NULL;
    }

    return;
}

SQLEXECUTERESULT_PTR SQLiteHandleImpl::ExecuteSQL(const std::string &kSQL)
{
    SQLEXECUTERESULT_PTR result(new SQLExecuteResultImpl());
    int nRetCode = false;

    int keep_while_loop = true;

    sqlite3_stmt *statement_handle = NULL;

    LockerCtrl locker_ctrl(mutex_);

    if (kLogger_)
    {
        kLogger_->WriteLogForExecuteSQL(kSQL);
    }

    nRetCode = sqlite3_prepare_v2(
        db_handle_,
        kSQL.c_str(),
        kSQL.size(),
        &statement_handle,
        NULL
    );
    if (nRetCode != SQLITE_OK)
    {
        // printf(sqlite3_errmsg(db_handle) );
        MY_PROCESS_ERROR(false);
    }

    while (keep_while_loop)
    {
        SqlResultLine new_line;
        nRetCode = sqlite3_step(statement_handle);
        switch (nRetCode)
        {
        case SQLITE_ROW:
            nRetCode = SaveOneLine(statement_handle, new_line);
            MY_PROCESS_ERROR(nRetCode);

            result->AppendLine(new_line);

            break;
        case SQLITE_DONE:
            // printf("==== return over ====\n");
            keep_while_loop = false;
            break;
        default:
            MY_PROCESS_ERROR(false);
            // printf("Error occurred. %s\n", sqlite3_errmsg(db_handle_) );
            keep_while_loop = false;
        }
    }

// Exit1:
    result->set_returned_code(true);
Exit0:
    if (statement_handle)
    {
        sqlite3_finalize(statement_handle);
        statement_handle = NULL;
    }
    return result;
}

void SQLiteHandleImpl::Lock()
{
    mutex_.Lock();
    return;
}

void SQLiteHandleImpl::Unlock()
{
    mutex_.Unlock();
    return;
}

int SQLiteHandleImpl::SaveOneLine(sqlite3_stmt *statement_handle, SqlResultLine &ret_line)
{
    int nResult  = false;
    int nRetCode = false;

    int row_count = sqlite3_column_count(statement_handle);

    ret_line.clear();

    for (int i = 0; i < row_count; i++)
    {
        int data_type = sqlite3_column_type(statement_handle, i);
        switch (data_type)
        {
        case SQLITE_INTEGER:
            {
                std::stringstream ss;
                ss << sqlite3_column_int(statement_handle, i);
                ret_line.push_back(ss.str() );
            }
            break;
        case SQLITE_FLOAT:
            ret_line.push_back(std::string("<float type>") );
            break;
        case SQLITE_BLOB:
            ret_line.push_back(std::string("<blob type>") );
            break;
        case SQLITE_NULL:
            ret_line.push_back(std::string("<null type>") );
            break;
        case SQLITE3_TEXT:
            // printf("type is text, value = %s", sqlite3_column_text(statement_handle, i) );
            ret_line.push_back(std::string((const char*)sqlite3_column_text(statement_handle, i) ) );
            break;
        default:
            ret_line.push_back(std::string("<unknown type>") );
        }
        // printf("\n");
    }

// Exit1:
    nResult = true;
// Exit0:
    return nResult;
}

