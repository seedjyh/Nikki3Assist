//
//   File name      : SQLiteLogger.cpp
//
//   Code by        : jiangyonghang
//
//   Project name   : IFSQLiteAdaptor
//
//   Create datetime: 2014-02-16 22:16:22
//

// Inherit header
#include "SQLiteLogger.h"

// C system headers
// ...

// C++ system headers
// ...

// Headers from other projects
// ...

// Headers of current project
// ...

void SQLiteLogger::WriteLogForOpen(const std::string &kFileName) const
{
    if (logging_switch_state_)
    {
        DoWriteLogForOpen(kFileName);
    }
    return;
}

void SQLiteLogger::WriteLogForClose() const
{
    if (logging_switch_state_)
    {
        DoWriteLogForClose();
    }
    return;
}

void SQLiteLogger::WriteLogForExecuteSQL(const std::string &kSQL) const
{
    if (logging_switch_state_)
    {
        DoWriteLogForExecuteSQL(kSQL);
    }
    return;
}
