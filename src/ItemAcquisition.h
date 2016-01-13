//
//   File name      : ItemAcquisition.h
//
//   Author         : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-11 13:43:22
//

#ifndef _ITEMACQUISITION_H_
#define _ITEMACQUISITION_H_ 1

// Inherite header
// ...

// C system headers
// ...

// C++ system headers
#include <boost/shared_ptr.hpp>

// Headers from other projects
// ...

// Headers of current project
// ...

class ItemAcquisition
{
public:
    ItemAcquisition(){}
    ~ItemAcquisition(){}

    void Print(){}
protected:
private:
};

typedef boost::shared_ptr<ItemAcquisition> ITEMACQUISITION_PTR;

#endif
