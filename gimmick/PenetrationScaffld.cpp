#include "PenetrationScaffld.h"

#include "../util/Model.h"

#include "../object/Player.h"

PenetrationScaffld::PenetrationScaffld(int handle, LoadObjectInfo objInfo):GimmickBase(handle,objInfo)
{
	//モデルのマテリアルのアルファ値を0にして透過させる
	materialNum_ = MV1GetMaterialNum(model_->GetModelHandle());

	//当たり判定を行わない
	isCollCheck_ = false;
}

PenetrationScaffld::~PenetrationScaffld()
{
}

void PenetrationScaffld::Update(Player& player)
{
	//カラー変数
	COLOR_F color = {};

	//アルファ値を増減する
	if (alphaValueIncrease_) {
		alphaValue_ = (std::min)(alphaValue_ + 0.001f, 1.0f);
		if (alphaValue_ == 1.0f) {
			alphaValueIncrease_ = false;
		}
	}
	else {
		alphaValue_ = (std::max)(alphaValue_ - 0.001f, 0.8f);
		if (alphaValue_ == 0.8f) {
			alphaValueIncrease_ = true;
		}
	}

	//マテリアルのアルファ値を変更する
	for (int i = 0; i < materialNum_; i++) {
		color = MV1GetMaterialDifColor(model_->GetModelHandle(), i);
		color.a = alphaValue_;
		MV1SetMaterialDifColor(model_->GetModelHandle(), i, color);
	}
}

void PenetrationScaffld::Draw()
{
	model_->Draw();
}
