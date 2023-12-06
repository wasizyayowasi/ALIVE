#pragma once
#include "GimmickBase.h"
#include <memory>

class ManualCrank;

class CrankScaffold : public GimmickBase
{
public:
	//コンストラクタ
	CrankScaffold(int handle, LoadObjectInfo objInfo);
	virtual ~CrankScaffold();

	void Update(Player& player, const InputState& input)override;
	void Draw();

	//衝突判定を行うモデルを追加する
	virtual std::shared_ptr<Model> AddCollModel();


private:

	float upVec_ = 0.0f;

	VECTOR initPos_ = {};


	std::shared_ptr<ManualCrank> crank_;

};

