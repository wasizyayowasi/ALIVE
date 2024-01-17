#pragma once
#include "CharacterBase.h"

class Model;

class DeadPerson : public CharacterBase
{
public:
	DeadPerson(int handle, LoadObjectInfo objInfo, int animNo);
	virtual ~DeadPerson();

	//初期化
	void Init();

	//更新
	void Update(Player& player)override;
	//描画
	void Draw();

	//衝突判定を行うモデルを追加する
	virtual std::shared_ptr<Model> AddCollModel();

private:

	void HitColl(Player& player);

private:

	int color_ = 0;
	int animNo_ = 0;

	MV1_COLL_RESULT_POLY_DIM hitDim_ = {};

};

