#include "HopStepJump.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/Easing.h"

#include <string>
#include <stdio.h>
#include <algorithm>

namespace {
	constexpr float total_time = 180.0f;
}

HopStepJump::HopStepJump(const int handle, const Material materialType, const LoadObjectInfo objInfo):GimmickBase(handle,materialType,objInfo)
{
	//マテリアルの数の取得
	materialNum_ = MV1GetMaterialNum(handle);

	//当たり判定の設定
	model_->SetUseCollision(true, true);

	//当たり判定の設定
	isCollCheck_ = true;

	//時間の設定
	elapseddTime_ = total_time;

	currentNum_ = StrUtil::GetNumberFromString(objInfo.name, ".");
}

HopStepJump::~HopStepJump()
{
}

void HopStepJump::Update(Player& player)
{
	//タイマー
	elapseddTime_ = (std::max)(elapseddTime_ - 1, 0.0f);

	//現在の番号を変更する
	if (elapseddTime_ == 0.0f) {
		currentNum_ = (std::min)(currentNum_ + 1, materialNum_);
		elapseddTime_ = total_time;
	}

	//マテリアルの数を超えたら0にする
	if (currentNum_ == materialNum_) {
		currentNum_ = 0;
	}

	//アルファ値を変更する
	if (elapseddTime_ >= 120) {
		alphaValue_ = (std::min)(alphaValue_ + (1.0f / 60.0f), 1.0f);
	}
	else if(elapseddTime_ <= 60){
		alphaValue_ = (std::max)(alphaValue_ - (1.0f / 60.0f), 0.0f);
	}

	COLOR_F color = {};

	//現在の番号以外のマテリアルのカラーのアルファ値を0にする
	for (int i = 0; i < materialNum_; i++) {
		color = MV1GetMaterialDifColor(model_->GetModelHandle(), i);

		if (currentNum_ == i) {
			color.a = alphaValue_;
		}
		else{
			color.a = 0.0f;
		}
		
		MV1SetMaterialDifColor(model_->GetModelHandle(), i, color);
	}

	std::string num = std::to_string(currentNum_);

	//当たり判定用フレームの変更
	model_->SetCollFrame(num.c_str());

}

void HopStepJump::Draw()
{
	//model_->Draw();

	MV1DrawFrame(model_->GetModelHandle(), model_->GetColFrameIndex());
}
