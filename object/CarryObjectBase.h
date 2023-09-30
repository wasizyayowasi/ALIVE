#pragma once
#include "DxLib.h"
#include <memory>

class Model;
class Player;

class CarryObjectBase
{
public:

	CarryObjectBase(const char* filename);
	CarryObjectBase(int handle);
	virtual ~CarryObjectBase();

	virtual void update();
	virtual void draw();

	virtual bool collInfo(Player& player);

protected:

	VECTOR pos_;

	MV1_COLL_RESULT_POLY_DIM hitDim_;

	std::shared_ptr<Model> object_;

};

