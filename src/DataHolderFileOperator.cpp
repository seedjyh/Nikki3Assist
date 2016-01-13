//
//   File name      : DataHolderFileOperator.cpp
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-11 15:11:16
//

// Implemented header
#include "DataHolderFileOperator.h"

// C system headers
// ...

// C++ system headers
// ...

// Headers from other projects
// ...

// Headers of current project
#include "IFDataType/IFException.h"
#include "IFDataType/DataHolder.h"
#include "IFMacros/BasicOperateMacro.h"
#include "IFDataType/Tstring.h"

DATAHOLDER_PTR DataHolderFileOperator::CreateFromFile(const Tstring &kFilePath)
{
    FILE *fp = _tfopen(kFilePath.c_str(), _T("rb"));
    if (NULL == fp)
    {
        throw IFException(std::string("file doesn\'t exist"));
    }
    if (fseek(fp, 0, SEEK_END) != 0)
    {
        throw IFException("seek file end failed");
    }
    int file_size = ftell(fp);

    if (fseek(fp, 0, SEEK_SET) != 0)
    {
        throw IFException("seek file beginning failed");
    }
    char *buffer = new char[file_size];
    int read_count = fread(buffer, 1, file_size, fp);
    MY_FCLOSE(fp);
    if (read_count != file_size)
    {
        throw IFException("read file failed");
    }
    DATAHOLDER_PTR result(new DataHolder(read_count, buffer));
    MY_DELETEARRAY(buffer);
    return result;
}

void DataHolderFileOperator::WriteToFile(const DATAHOLDER_PTR &kData, const Tstring &kFilePath)
{
    FILE *fp = _tfopen(kFilePath.c_str(), _T("wb"));
    if (NULL == fp)
    {
        throw IFException(std::string("Can\'t open file to write"));
    }
    int write_count = fwrite(kData->content(), 1, kData->size(), fp);
    MY_FCLOSE(fp);
    if (write_count != kData->size())
    {
        throw IFException(std::string("write data to file failed"));
    }

    return;
}
