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
	void Update(Player& player, const InputState& input)override;
	//�`��
	void Draw();

	//�Փ˔�����s�����f����ǉ�����
	virtual std::shared_ptr<Model> AddCollModel();

private:

	void HitColl(Player& player);

private:

	int color;

	VECTOR temp;

	MV1_COLL_RESULT_POLY_DIM hitDim_ = {};

};

