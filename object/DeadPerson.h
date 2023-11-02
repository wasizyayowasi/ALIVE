#pragma once
#include "CharacterBase.h"

class Model;

class DeadPerson : public CharacterBase
{
public:

	DeadPerson(const char* const filename, LoadObjectInfo objInfo,int animNo);
	DeadPerson(int handle, LoadObjectInfo objInfo, int animNo);
	virtual ~DeadPerson();

	//�X�V
	void Update(Player& player)override;
	//�`��
	void Draw();

	using ObjectBase::Draw;

private:

	void HitColl(Player& player);

private:

	MV1_COLL_RESULT_POLY_DIM hitDim_ = {};

};

