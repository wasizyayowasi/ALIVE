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

	void update(Player& player);
	void draw();

private:

};

