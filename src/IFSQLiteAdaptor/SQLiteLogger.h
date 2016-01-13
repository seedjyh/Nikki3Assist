//
//   File name      : SQLiteLogger.h
//
//   Code by        : jiangyonghang
//
//   Project name   : IFSQLiteAdaptor
//
//   Create datetime: 2014-02-07 13:40:56
//

#ifndef _IFSQLITEADAPTOR_INCLUDE_SQLITELOGGER_H_
#define _IFSQLITEADAPTOR_INCLUDE_SQLITELOGGER_H_ 1

// Inherit header
// ...

// C system headers
// ...

// C++ system headers
#include <string>

// Headers from other projects
// ...

// Headers of current project
// ...

class SQLiteLogger
{
    int logging_switch_state_;
public:
    SQLiteLogger():logging_switch_state_(false){};
    virtual ~SQLiteLogger(){};

    void set_logging_switch_state(int state) {logging_switch_state_ = state; return;}

    void WriteLogForOpen(const std::string &kFileName) const;
    void WriteLogForClose() const;
    void WriteLogForExecuteSQL(const std::string &kSQL) const;

protected:
private:
    virtual void DoWriteLogForOpen(const std::string &kFileName) const = 0;
    virtual void DoWriteLogForClose() const = 0;
    virtual void DoWriteLogForExecuteSQL(const std::string &kSQL) const = 0;
};

#endif

