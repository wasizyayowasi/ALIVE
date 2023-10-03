#pragma once
#include "GimmickBase.h"
#include <memory>
#include <vector>

class Model;

class Steelyard : public GimmickBase
{
public:
	Steelyard(const char* const filename);
	Steelyard(int handle);
	virtual ~Steelyard();

	void update();
	void draw();

private:

};

