#include "Lever.h"
#include "../util/Model.h"
#include "../util/Util.h"
#include "../util/ExternalFile.h"

namespace {
	const char* const filename = "data/model/other/mv1/Lever.mv1";
}

//コンストラクタ
Lever::Lever(LoadObjectInfo info, VECTOR stopPos)
{
	//モデルクラスのインスタンス化
	model_ = std::make_shared<Model>(filename);

	//ポジションの初期化
	pos_ = info.pos;

	//エレベーターを停止させるポジション
	elevatorStopPosition_ = stopPos;

	//モデルの初期化
	model_->SetPos(pos_);
	model_->SetScale(info.scale);
	model_->SetRot(info.rot);

	//クランクを回す立ち位置を取得
	int standingNum = StrUtil::GetNumberFromString(info.name, ".");
	std::string standingName = StrUtil::GetConcatenateNumAndStrings("LeverStandingPos", ".", standingNum);

	//立つ位置の初期化
	standingPos_ = ExternalFile::GetInstance().GetSpecifiedGimmickInfo(pos_, standingName.c_str()).pos;

}

//デストラクタ
Lever::~Lever()
{
}

//更新
void Lever::Update()
{
	//更新
	model_->Update();

	//アニメーションを停止アニメーションにする
	if (model_->IsAnimEnd()) {
		OffAnimation();
	}
}

//描画
void Lever::Draw()
{
	//描画
	model_->Draw();
}

//衝突判定
bool Lever::CollCheck(VECTOR playerPos)
{
	//プレイヤーとレバーモデルの衝突判定
	auto result = MV1CollCheck_Sphere(model_->GetModelHandle(), model_->GetColFrameIndex(), playerPos, 50.0f);

	if (result.HitNum > 0) {
		MV1CollResultPolyDimTerminate(result);
		return true;
	}

	return false;
}

//レバーの起動
void Lever::OnAnimation()
{
	model_->ChangeAnimation(static_cast<int>(AnimType::on), false, false, 10);
	isOn_ = true;
}

//レバーの停止
void Lever::OffAnimation()
{
	model_->ChangeAnimation(static_cast<int>(AnimType::off), false, false, 10);
	isOn_ = false;
}