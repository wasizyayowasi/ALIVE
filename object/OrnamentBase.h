#pragma once
#include "ObjectBase.h"
class OrnamentBase : public ObjectBase
{
public:
	OrnamentBase(const char* const filename, LoadObjectInfo objInfo);
	OrnamentBase(int handle, LoadObjectInfo objInfo);
	virtual ~OrnamentBase();

	virtual void Update(Player& player)override;
	virtual void Draw();

private:

};

