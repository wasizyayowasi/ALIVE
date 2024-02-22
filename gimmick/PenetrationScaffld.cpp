#include "PenetrationScaffld.h"

#include "../util/Model.h"

#include "../object/Player.h"

PenetrationScaffld::PenetrationScaffld(const int handle,const Material materialType,const LoadObjectInfo& objInfo):GimmickBase(handle, materialType, objInfo)
{
	//モデルのマテリアルのアルファ値を0にして透過させる
	materialNum_ = MV1GetMaterialNum(model_->GetModelHandle());

	//当たり判定を行わない
	isCollCheck_ = false;

	//カラー変数
	COLOR_F color = {};

	//マテリアルのアルファ値を変更する
	for (int i = 0; i < materialNum_; i++)
	{
		color = MV1GetMaterialDifColor(model_->GetModelHandle(), i);
		color.a = alphaValue_;
		MV1SetMaterialDifColor(model_->GetModelHandle(), i, color);
	}
}

PenetrationScaffld::~PenetrationScaffld()
{
}

void PenetrationScaffld::Update(Player& player)
{
}

void PenetrationScaffld::Draw()
{
	model_->Draw();
}
