//
//   File name      : MMLScriptFileFormatErrorException.h
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-12 14:30:48
//

#ifndef _MMLSCRIPTFILEFORMATERROREXCEPTION_H_
#define _MMLSCRIPTFILEFORMATERROREXCEPTION_H_ 1

// Inherited header
#include "IFDataType/IFException.h"

// C system headers
// ...

// C++ system headers
// ...

// Headers from other projects
// ...

// Headers of current project
#include "IFOperator/CodeTransformer.h"

class MMLScriptFileFormatErrorException: public IFException
{
public:
    MMLScriptFileFormatErrorException(const Tstring &kScriptFilePath, const std::string &kDetails)
    :   IFException(std::string("MML script file \"") + CodeTransformer::TransTstringToString(kScriptFilePath) + "\" has format error: " + kDetails){}
    virtual ~MMLScriptFileFormatErrorException(){}
protected:
private:
};

#endif
