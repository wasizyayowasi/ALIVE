#pragma once
#include "ObjectBase.h"

class OrnamentBase : public ObjectBase
{
public:
	OrnamentBase(int handle, LoadObjectInfo objInfo);
	virtual ~OrnamentBase();

	//XV
	virtual void Update(Player& player)override;
	//•`‰æ
	virtual void Draw();

private:

};

