#pragma once
#include "OrnamentBase.h"

class CarryObjectBase:public OrnamentBase
{
public:

	CarryObjectBase(const char* filename);
	CarryObjectBase(int handle);
	virtual ~CarryObjectBase();

	virtual void update(Player& player)override;
	virtual void draw();

	virtual bool collInfo();

protected:

	VECTOR pos_;

	MV1_COLL_RESULT_POLY_DIM hitDim_;

};

