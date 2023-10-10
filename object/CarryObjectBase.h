#pragma once
#include "OrnamentBase.h"

class CarryObjectBase:public OrnamentBase
{
public:

	CarryObjectBase(const char* filename);
	CarryObjectBase(int handle);
	virtual ~CarryObjectBase();

	virtual void Update(Player& player)override;
	virtual void Draw();

	virtual bool CollInfo();

protected:

	VECTOR pos_;

	MV1_COLL_RESULT_POLY_DIM hitDim_;

};

