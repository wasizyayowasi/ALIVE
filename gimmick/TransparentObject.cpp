#include "TransparentObject.h"
#include "Switch.h"
#include "../util/Model.h"
#include "../util/ExternalFile.h"
#include "../util/Util.h"
#include <algorithm>

TransparentObject::TransparentObject(int handle, LoadObjectInfo objInfo) : GimmickBase(handle, objInfo)
{
	//衝突判定用フレームの設定
	model_->SetUseCollision(true, false);

	//文字列の末尾を数値として取得する
	int num = StrUtil::GetNumberFromString(objInfo.name, ".");
	//第一引数と数値を連結させた文字列を取得
	std::string name = StrUtil::GetConcatenateNumAndStrings("TransSwitch", ".", num);

	//データの取得
	auto info = ExternalFile::GetInstance().GetSpecifiedGimmickInfo(objInfo.pos, name.c_str());
	//スイッチのインスタンス化
	switch_ = std::make_shared<Switch>(info);

	//衝突判定を行わない状態で初期化
	isCollCheck_ = false;

	//モデルのマテリアルのアルファ値を0にして透過させる
	materialNum_ = MV1GetMaterialNum(model_->GetModelHandle());

	COLOR_F color = {};

	for (int i = 0; i < materialNum_;i++) {
		color = MV1GetMaterialDifColor(model_->GetModelHandle(), i);
		color.a = 0.0f;
		MV1SetMaterialDifColor(model_->GetModelHandle(), i, color);
	}

}

TransparentObject::~TransparentObject()
{
}

void TransparentObject::Update(Player& player)
{
	switch_->Update(player);

	//スイッチが起動していたら
	//モデルとの衝突判定を行うようにする
	if (switch_->TransCollResult()) {
		isCollCheck_ = true;
	}
	else {
		isCollCheck_ = false;
	}

	//アルファ値を変更する
	if (isCollCheck_) {
		alphaValue_ = (std::min)(alphaValue_ + 0.1f, 1.0f);
	}
	else {
		alphaValue_ = (std::max)(alphaValue_ - 0.1f, 0.0f);
	}

	COLOR_F color = {};

	for (int i = 0; i < materialNum_; i++) {
		color = MV1GetMaterialDifColor(model_->GetModelHandle(), i);
		color.a = alphaValue_;
		MV1SetMaterialDifColor(model_->GetModelHandle(), i, color);
	}
}

void TransparentObject::Draw()
{
	model_->Draw();

	switch_->Draw();
}

void TransparentObject::UpdateForCorpse(std::shared_ptr<ObjectBase> deadPerson)
{
	switch_->HitColl(deadPerson);
}

std::shared_ptr<Model> TransparentObject::AddCollModel()
{
	return switch_->GetModelPointer();
}
