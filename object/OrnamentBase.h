#pragma once
#include "ObjectBase.h"
class OrnamentBase : public ObjectBase
{
public:
	OrnamentBase(const char* const filename, LoadObjectInfo objInfo);
	OrnamentBase(int handle, LoadObjectInfo objInfo);
	virtual ~OrnamentBase();

	//XV
	virtual void Update(Player& player)override;
	//•`‰æ
	virtual void Draw();

	//Õ“Ë”»’è
	virtual void HitColl(std::shared_ptr<ObjectBase> pointer);

private:

};

