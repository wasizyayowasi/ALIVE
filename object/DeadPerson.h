#pragma once
#include "CharacterBase.h"

class Model;

class DeadPerson : public CharacterBase
{
public:
	DeadPerson(int handle, LoadObjectInfo objInfo, int animNo);
	virtual ~DeadPerson();

	//������
	void Init();

	//�X�V
	void Update(Player& player)override;
	//�`��
	void Draw();

	//�Փ˔�����s�����f����ǉ�����
	virtual std::shared_ptr<Model> AddCollModel();

private:

	void HitColl(Player& player);

private:

	int color_ = 0;
	int animNo_ = 0;

	MV1_COLL_RESULT_POLY_DIM hitDim_ = {};

};

