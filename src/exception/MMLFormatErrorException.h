//
//   File name      : MMLFormatErrorException.h
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-12 14:21:32
//

#ifndef _MMLFORMATERROREXCEPTION_H_
#define _MMLFORMATERROREXCEPTION_H_ 1

// Inherited header
#include "IFDataType/IFException.h"

// C system headers
// ...

// C++ system headers
// ...

// Headers from other projects
// ...

// Headers of current project
// ...

class MMLFormatErrorException : public IFException
{
    std::string text_;
    std::string details_;
public:
    MMLFormatErrorException(const std::string &kText, const std::string &kDetails)
    :   IFException(std::string("MML format error, command=\"") + kText + "\", details=" + kDetails),
        text_(kText),
        details_(kDetails){}
    virtual ~MMLFormatErrorException(){}

    const std::string& text() const { return text_; }
    const std::string& details() const { return details_; }
protected:
private:
};

#endif
