//
//   File name      : SQLExecuteResult.h
//
//   Code by        : jiangyonghang
//
//   Project name   : IFSQLiteAdaptor
//
//   Create datetime: 2013-02-16 14:00:06
//

#ifndef _IFSQLiteAdaptor_include_SQLExecuteResult_h_
#define _IFSQLiteAdaptor_include_SQLExecuteResult_h_

// Inherit header
// ...

// C system headers
// ...

// C++ system headers
#include <boost/shared_ptr.hpp>

// Headers from other projects
// ...

// Headers of current project
#include "SQLResultDataStructure.h"

class SQLExecuteResult
{
public:
    SQLExecuteResult(){};
    virtual ~SQLExecuteResult(){};

    virtual void set_returned_code(int code) = 0;
    virtual void AppendLine(const SqlResultLine &kNewLine) = 0;

    virtual int returned_code() const = 0;

    virtual int GetLineCount() const = 0;

    virtual const std::string& GetResult(int line_index, int column_index) const = 0;

protected:
private:
};

typedef boost::shared_ptr<SQLExecuteResult> SQLEXECUTERESULT_PTR;

#endif

