#pragma once
//#include "GimmickBase.h"
#include <memory>
#include <vector>

class Model;

class Steelyard// : public GimmickBase
{
public:
	Steelyard();
	virtual ~Steelyard();

	void update();
	void draw();

private:

	std::vector<std::shared_ptr<Model>> model_;

};

