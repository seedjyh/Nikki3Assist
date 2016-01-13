//
//   File name      : SQLExecuteResultImpl.h
//
//   Code by        : jiangyonghang
//
//   Project name   : IFSQLiteAdaptor
//
//   Create datetime: 2013-02-16 17:14:39
//

#ifndef _IFSQLiteAdaptor_src_SQLExecuteResultImpl_h_
#define _IFSQLiteAdaptor_src_SQLExecuteResultImpl_h_

// Inherit header
#include "SQLExecuteResult.h"

// C system headers
// ...

// C++ system headers
#include <string>
#include <vector>

// Headers from other projects
// ...

// Headers of current project
// ...

class SQLExecuteResultImpl: public SQLExecuteResult
{
    int returned_code_;
    SqlResultTable result_table_;
public:
    SQLExecuteResultImpl():returned_code_(false){};
    virtual ~SQLExecuteResultImpl(){};

    virtual void set_returned_code(int code) {returned_code_ = code; return;}
    virtual void AppendLine(const SqlResultLine &kNewLine) { result_table_.push_back(kNewLine); return; }

    virtual int returned_code() const {return returned_code_;}

    virtual int GetLineCount() const {return result_table_.size();};

    virtual const std::string& GetResult(int line_index, int column_index) const
    {
        assert(line_index >= 0);
        assert(line_index < result_table_.size());
        assert(column_index >= 0);
        if (column_index >= result_table_[line_index].size())
        {
            throw 1;
        }
        return result_table_[line_index][column_index];
    }

protected:
private:
};

#endif

