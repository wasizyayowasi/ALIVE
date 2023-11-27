#pragma once
#include "CharacterBase.h"

class Model;

class DeadPerson : public CharacterBase
{
public:

	DeadPerson(const char* const filename, LoadObjectInfo objInfo,int animNo);
	DeadPerson(int handle, LoadObjectInfo objInfo, int animNo);
	virtual ~DeadPerson();

	//更新
	void Update(Player& player, const InputState& input)override;
	//描画
	void Draw();

	//衝突判定を行うモデルを追加する
	virtual std::shared_ptr<Model> AddCollModel();

private:

	void HitColl(Player& player);

private:

	int color;

	VECTOR temp;

	MV1_COLL_RESULT_POLY_DIM hitDim_ = {};

};

