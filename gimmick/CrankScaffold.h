#pragma once
#include "GimmickBase.h"
#include <memory>

class ManualCrank;

class CrankScaffold : public GimmickBase
{
public:
	//コンストラクタ
	CrankScaffold(int handle, LoadObjectInfo objInfo);
	//デストラクタ
	virtual ~CrankScaffold();

	//更新
	void Update(Player& player, const InputState& input)override;
	//描画
	void Draw();

	//衝突判定を行うモデルを追加する
	virtual std::shared_ptr<Model> AddCollModel();
private:

	float upVec_ = 0.0f;						//上昇ベクトル

	VECTOR initPos_ = {};						//初期ポジション

	std::shared_ptr<ManualCrank> crank_;		//クランククラスのスマートポインタ

};

