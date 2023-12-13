#include "ManualCrank.h"
#include "../util/Model.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/Util.h"
#include "../object/Player.h"
#include <algorithm>
#include <math.h>


namespace {
	//クランクモデルのファイルパス
	const char* const crank_filename = "data/model/other/mv1/Crank.mv1";

	//最大回転値
	constexpr float max_rot_Z = -630.0f;
}

//コンストラクタ
ManualCrank::ManualCrank(LoadObjectInfo objInfo)
{
	//モデルクラスの初期化
	model_ = std::make_shared<Model>(crank_filename);
	model_->SetCollFrame();
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
bool ManualCrank::HitCollPlayer(Player& player)
{
	
	MV1_COLL_RESULT_POLY_DIM result;

	VECTOR playerPos = player.GetStatus().pos;

	MV1RefreshCollInfo(model_->GetModelHandle(), model_->GetColFrameIndex());
	result = MV1CollCheck_Capsule(model_->GetModelHandle(), model_->GetColFrameIndex(), playerPos, VAdd(playerPos, VGet(0, player.GetStatus().height, 0)),80.0f);

	if (result.HitNum < 1) {
		MV1CollResultPolyDimTerminate(result);
		return false;
	}

	MV1CollResultPolyDimTerminate(result);

	return true;
}

//Z軸の最大回転率を取得する
float ManualCrank::GetMaxRotZ()
{
	return max_rot_Z;
}
