#pragma once
#include "CarryObjectBase.h"

class Player;

class cube : public CarryObjectBase
{
public:

	cube(const char* const filename, LoadObjectInfo objInfo);
	cube(int handle, LoadObjectInfo objInfo);
	virtual ~cube();

private:


};

