//
//   File name      : NumberOperator.cpp
//
//   Code by        : jiangyonghang
//
//   Project name   : IFOperator
//
//   Create datetime: 2011-03-31 12:35:00
//

// Tested or implemented header
#include "NumberOperator.h"

// C system headers
#include <stdio.h>
#pragma warning(disable:4996)

// C++ system headers
// ...

// Headers from other projects
#include "IFMacros/BasicOperateMacro.h"
#include "IFMacros/BasicProcessMacro.h"
#include "IFDataType/Tstring.h"

// Headers of current project
#include "IFDataType/IFException.h"

int NumberOperator::ItoA(int num, const char kFormat[], string &strnum)
{
    int nResult  = false;
    int nRetCode = false;

    char buffer[1024] = {'\n'};

    nRetCode = _snprintf(buffer, MY_SIZE_OF_ARRAY(buffer), kFormat, num);
    MY_PROCESS_ERROR(nRetCode > 0);

    MY_SET_TAIL_NULL(buffer);

    strnum = string(buffer);

// Exit1:
    nResult = true;
Exit0:
    return nResult;
}

int NumberOperator::ItoA(int num, const Tstring &kFormat, Tstring &strnum)
{
    int nResult  = false;
    int nRetCode = false;

    TCHAR buffer[1024] = {'\n'};

    nRetCode = _sntprintf(buffer, MY_SIZE_OF_ARRAY(buffer), kFormat.c_str(), num);
    MY_PROCESS_ERROR(nRetCode > 0);

    MY_SET_TAIL_NULL(buffer);

    strnum = Tstring(buffer);

// Exit1:
    nResult = true;
Exit0:
    return nResult;
}

int NumberOperator::DtoA(double num, const char kFormat[], string &strnum)
{
    int nResult  = false;
    int nRetCode = false;

    char buffer[1024] = {'\n'};

    nRetCode = _snprintf(buffer, MY_SIZE_OF_ARRAY(buffer), kFormat, num);
    MY_PROCESS_ERROR(nRetCode > 0);

    MY_SET_TAIL_NULL(buffer);

    strnum = string(buffer);

// Exit1:
    nResult = true;
Exit0:
    return nResult;
}


int NumberOperator::DtoA(double num, const Tstring &kFormat, Tstring &strnum)
{
    int nResult  = false;
    int nRetCode = false;

    TCHAR buffer[1024] = {TCHAR('\n')};

    nRetCode = _sntprintf(buffer, MY_SIZE_OF_ARRAY(buffer), kFormat.c_str(), num);
    MY_PROCESS_ERROR(nRetCode > 0);

    MY_SET_TAIL_NULL(buffer);

    strnum = Tstring(buffer);

// Exit1:
    nResult = true;
Exit0:
    return nResult;
}

int NumberOperator::AtoI(const std::string &kStrNum)
{
    int result = 0;
    if (sscanf(kStrNum.c_str(), "%d", &result) != 1)
    {
        throw IFException(std::string("\"") + kStrNum + "\" is NaN");
    }
    return result;
}
