#pragma once
#include "GimmickBase.h"
#include <memory>
#include <vector>

class Model;

class Steelyard : public GimmickBase
{
public:
	Steelyard();
	virtual ~Steelyard();

	const char* getClassName() { return "Steelyard"; }

	void update();
	void draw();

private:

};

