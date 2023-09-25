#pragma once
#include "DxLib.h"
#include <memory>

class Model;
class Switch;

class GimmickBase
{
	friend Switch;
public:

	GimmickBase(const char* filename);
	GimmickBase(int handle);
	virtual ~GimmickBase();

	virtual void update();
	void draw();

private:

	VECTOR pos_;

	std::shared_ptr<Model> model_;

};

