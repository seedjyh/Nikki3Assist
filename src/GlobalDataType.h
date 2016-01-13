//
//   File name      : GlobalDataType.h
//
//   Code by        : Yonghang Jiang(seedjyh@gmail.com)
//
//   Project name   : Nikki3Assist
//
//   Create datetime: 2016-01-10 07:13:36
//

#ifndef _GLOBALDATATYPE_H_
#define _GLOBALDATATYPE_H_ 1

// Inherit header
// ...

// C system headers
// ...

// C++ system headers
#include <map>

// Headers from other projects
// ...

// Headers of current project
// ...

typedef int ItemTypeID;

typedef int ItemID;
typedef std::map<ItemID, int> Materials;

typedef ItemID ItemIDInGame;

typedef int TaskID;

#endif
