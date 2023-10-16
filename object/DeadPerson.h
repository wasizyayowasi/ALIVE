#pragma once
#include "CharacterBase.h"

class Model;

class DeadPerson : public CharacterBase
{
public:

	DeadPerson(const char* const filename, LoadObjectInfo objInfo,int animNo);
	DeadPerson(int handle, LoadObjectInfo objInfo, int animNo);
	virtual ~DeadPerson();

	void Update(Player& player)override;
	void Draw();

	void NormalUpdate(Player& player);
	void DuringTransportUpdate(Player& player);

private:

	void HitColl(Player& player);

	MV1_COLL_RESULT_POLY_DIM hitDim_;

	void(DeadPerson::* updateFunc_)(Player& player);

};

