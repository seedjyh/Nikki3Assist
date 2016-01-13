//
//   File name      : DataHolderFileOperator.h
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-11 13:56:34
//

#ifndef _DATAHOLDERFILEOPERATOR_H_
#define _DATAHOLDERFILEOPERATOR_H_ 1

// Inherite header
// ...

// C system headers
// ...

// C++ system headers
#include <string>

// Headers from other projects
// ...

// Headers of current project
#include "IFDataType/DataHolder.h"

class Tstring;

class DataHolderFileOperator
{
public:
    static DATAHOLDER_PTR CreateFromFile(const Tstring &kFilePath);
    static void WriteToFile(const DATAHOLDER_PTR &kData, const Tstring &kFilePath);
protected:
private:
    DataHolderFileOperator(){}
    ~DataHolderFileOperator(){}
};

#endif
