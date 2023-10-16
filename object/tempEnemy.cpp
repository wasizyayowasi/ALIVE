#include "tempEnemy.h"
#include "../util/Model.h"
#include "../object/Player.h"

namespace {
	//追跡を止める範囲
	constexpr float range_to_stop_tracking = 30.0f;
}

tempEnemy::tempEnemy(const char* const filename, LoadObjectInfo objInfo):EnemyBase(filename,objInfo)
{
	model_->SetScale(objInfo.scale);
	model_->SetPos(objInfo.pos);
	model_->SetRot(objInfo.rot);
}

tempEnemy::tempEnemy(int handle, LoadObjectInfo objInfo) : EnemyBase(handle,objInfo)
{
	model_->SetScale(objInfo.scale);
	model_->SetPos(objInfo.pos);
	model_->SetRot(objInfo.rot);
	scale_ = objInfo.scale;
}

tempEnemy::~tempEnemy()
{
}

void tempEnemy::Update(Player& player)
{
	model_->Update();

	//プレイヤーと敵の座標差を見て、
	VECTOR between = VSub(player.GetStatus().pos, pos_);
	if (VSize(between) < range_to_stop_tracking) {
		return;
	}

	//プレイヤーを追跡する
	TrackingUpdate(player);
}

void tempEnemy::Draw()
{
	model_->Draw();
}

void tempEnemy::TrackingUpdate(Player& player)
{
	//プレイヤーと自分の差を算出し、正規化し、スピードを掛ける
	VECTOR moveVec = VScale(VNorm(VSub(player.GetStatus().pos, pos_)), 3.0f);

	//回転行列の取得
	MATRIX rotMtx = MGetRotVec2(VGet(0, 0, -1), VSub(player.GetStatus().pos, pos_));

	//拡縮行列の取得
	MATRIX scaleMtx = MGetScale(scale_);

	//回転行列と拡縮行列の掛け算
	MATRIX mtx = MMult(rotMtx, scaleMtx);

	//ポジションの移動
	pos_ = VAdd(pos_, moveVec);

	//回転行列と拡縮行列を掛けた行列に平行移動行列を書ける
	mtx = MMult(mtx, MGetTranslate(pos_));

	//行列をモデルにセットする
	MV1SetMatrix(model_->GetModelHandle(), mtx);
}
