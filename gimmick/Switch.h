#pragma once
#include "GimmickBase.h"

class Switch : public GimmickBase
{
public:

	Switch();
	virtual ~Switch();

	const char* getClassName() { return "Switch"; }

	void update();
	void draw();

private:



};

