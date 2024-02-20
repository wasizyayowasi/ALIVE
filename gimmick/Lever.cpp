#include "Lever.h"

#include "../util/Model.h"
#include "../util/Util.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"
#include "../util/ModelManager.h"

namespace 
{
	//アニメーションの変更時間
	constexpr int anim_change_time = 10;

	//衝突判定用球の半径
	constexpr float radius_sphere = 100.0f;
}

//コンストラクタ
Lever::Lever(const LoadObjectInfo info)
{
	//短縮化
	auto& file = ExternalFile::GetInstance();
	auto& model = ModelManager::GetInstance();

	//モデルクラスのインスタンス化
	model_ = std::make_shared<Model>(model.GetModelHandle(objData[static_cast<int>(ObjectType::Lever)].name), Material::Iron);

	//ポジションの初期化
	pos_ = info.pos;

	//モデルの初期化
	model_->SetPos(pos_);
	model_->SetScale(info.scale);
	model_->SetRot(info.rot);

	//レバーがどのエレベーターグループに属しているか取得する
	int standingNum = StrUtil::GetNumberFromString(info.name, ".");
	std::string standingName = StrUtil::GetConcatenateNumAndStrings("LeverStandingPos", ".", standingNum);

	//取得したグループの何番目のレバーかを取得する
	int num = StrUtil::GetNumberFromString(info.name, "-");
	standingName = StrUtil::GetConcatenateNumAndStrings(standingName, "-", num);

	//立つ位置の初期化
	standingPos_ = file.GetSpecifiedGimmickInfo(pos_, standingName.c_str()).pos;
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
bool Lever::CollCheck(const VECTOR playerPos) const
{
	//プレイヤーとレバーモデルの衝突判定
	auto result = MV1CollCheck_Sphere(model_->GetModelHandle(), model_->GetColFrameIndex(), playerPos, radius_sphere);

	if (result.HitNum > 0)
	{
		MV1CollResultPolyDimTerminate(result);
		return true;
	}

	return false;
}

//レバーの起動
void Lever::OnAnimation()
{
	model_->ChangeAnimation(static_cast<int>(AnimType::on), false, false, anim_change_time);
	isOn_ = true;
}

//レバーの停止
void Lever::OffAnimation()
{
	model_->ChangeAnimation(static_cast<int>(AnimType::off), false, false, anim_change_time);
	isOn_ = false;
}