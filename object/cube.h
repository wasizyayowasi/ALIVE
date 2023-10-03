#pragma once
#include "CarryObjectBase.h"

class Player;

class cube : public CarryObjectBase
{
public:

	cube(const char* const filename);
	cube(int handle);
	virtual ~cube();

private:


};

