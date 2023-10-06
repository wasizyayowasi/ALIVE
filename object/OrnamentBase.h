#pragma once
#include "ObjectBase.h"
class OrnamentBase : public ObjectBase
{
public:
	OrnamentBase(const char* const filename);
	OrnamentBase(int handle);
	virtual ~OrnamentBase();

	virtual void update(Player& player)override;
	virtual void draw();

private:

};

