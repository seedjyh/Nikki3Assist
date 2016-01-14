//
//   File name      : ItemNamePair.h
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-14 10:17:12
//

#ifndef _ITEMNAMEPAIR_H_
#define _ITEMNAMEPAIR_H_ 1

// Inherited header
// ...

// C system headers
// ...

// C++ system headers
#include <string>

// Headers from other projects
// ...

// Headers of current project
// ...

class ItemNamePair
{
    std::string name_;
    std::string type_name_;
public:
    ItemNamePair(){}
    ItemNamePair(const std::string &kName, const std::string &kTypeName)
    :   name_(kName),
        type_name_(kTypeName){}
    ~ItemNamePair(){}

    const std::string& name() const { return name_; }
    const std::string& type_name() const { return type_name_; }
protected:
private:
};

#endif
