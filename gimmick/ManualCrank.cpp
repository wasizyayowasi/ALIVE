#include "ManualCrank.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../object/Player.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/ModelManager.h"

#include <algorithm>
#include <math.h>


namespace
{
	//最大回転値
	constexpr float max_rot_Z = -630.0f;

	//衝突判定用カプセルの半径
	constexpr float radius_capsule = 60.0f;

	//最小衝突数
	constexpr int min_hit_num = 1;
}

//コンストラクタ
ManualCrank::ManualCrank(const LoadObjectInfo objInfo)
{
	//モデルクラスの初期化
	model_ = std::make_shared<Model>(ModelManager::GetInstance().GetModelHandle(objData_[static_cast<int>(ObjectType::Crank)].name), Material::Iron);
	model_->SetUseCollision(true,false);
	model_->SetScale(objInfo.scale);
	model_->SetPos(objInfo.pos);

	//ポジションの初期化
	pos_ = objInfo.pos;

	//クランクを回す立ち位置を取得
	int standingNum = StrUtil::GetNumberFromString(objInfo.name, ".");
	std::string standingName = StrUtil::GetConcatenateNumAndStrings("StandingPosition", ".", standingNum);

	//立つ位置の初期化
	standingPos_ = ExternalFile::GetInstance().GetSpecifiedGimmickInfo(pos_, standingName.c_str()).pos;

}

//デストラクタ
ManualCrank::~ManualCrank()
{
}

//描画
void ManualCrank::Draw()
{
	//描画
	model_->Draw();
}

//プレイヤーとの衝突判定
bool ManualCrank::HitCollPlayer(Player& player) const
{
	
	MV1_COLL_RESULT_POLY_DIM result;

	VECTOR playerPos = player.GetStatus().pos;

	MV1RefreshCollInfo(model_->GetModelHandle(), model_->GetColFrameIndex());
	result = MV1CollCheck_Sphere(model_->GetModelHandle(), model_->GetColFrameIndex(), playerPos, radius_capsule);

	if (result.HitNum < min_hit_num)
	{
		MV1CollResultPolyDimTerminate(result);
		return false;
	}

	MV1CollResultPolyDimTerminate(result);

	return true;
}

//Z軸の最大回転率を取得する
float ManualCrank::GetMaxRotZ() const
{
	return max_rot_Z;
}
