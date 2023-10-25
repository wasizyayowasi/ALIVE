#include "EnemyBase.h"
#include "Player.h"
#include "../util/Model.h"


namespace {
	//追跡を止める範囲
	constexpr float range_to_stop_tracking = 60.0f;

	//モデルの初期回転ベクトル
	const VECTOR init_rot = { 0.0f,0.0f,-1.0f };

	//敵の視野角
	constexpr float viewing_angle = 30.0f;
	constexpr float visible_range = 200.0f;
}


EnemyBase::EnemyBase(const char* fileName, LoadObjectInfo objInfo):CharacterBase(fileName,objInfo)
{
}

EnemyBase::EnemyBase(int handle, LoadObjectInfo objInfo) : CharacterBase(handle,objInfo)
{
	model_->SetAnimation(0, true, false);
}

void EnemyBase::Update(Player& player)
{
	//プレイヤーの座標
	VECTOR playerPos = player.GetStatus().pos;

	//索敵
	SearchForPlayer(playerPos);

	//モデルの更新
	model_->Update();

	//プレイヤーを追跡する
	TrackingUpdate(playerPos);

	if (distance_ < range_to_stop_tracking + 20.0f) {
		ThrustAway(player);
	}

}

void EnemyBase::TrackingUpdate(VECTOR playerPos)
{

	//プレイヤーと敵の座標差を見て、
	if (distance_ < range_to_stop_tracking) {
		model_->ChangeAnimation(0, true, false, 20);
		return;
	}
	//敵の視野角よりも外側にいるまたは
	//敵からプレイヤーの距離が指定範囲より大きかったらreturn
	if (innerProduct > viewing_angle || distance_ > visible_range) {
		model_->ChangeAnimation(0, true, false, 20);
		return;
	}

	model_->ChangeAnimation(7, true, false, 20);

	//プレイヤーと自分の差を算出し、正規化し、スピードを掛ける
	VECTOR moveVec = VScale(VNorm(VSub(playerPos, pos_)), 3.0f);

	//回転行列の取得
	MATRIX rotMtx = MGetRotVec2(VGet(0, 0, -1), VSub(playerPos, pos_));

	//拡縮行列の取得
	MATRIX scaleMtx = MGetScale(scale_);

	//正面ベクトルを取得する
	frontVector_ = VTransformSR(VGet(0, 0, -1), rotMtx);

	//回転行列と拡縮行列の掛け算
	MATRIX mtx = MMult(rotMtx, scaleMtx);

	//ポジションの移動
	pos_ = VAdd(pos_, moveVec);

	//回転行列と拡縮行列を掛けた行列に平行移動行列を書ける
	mtx = MMult(mtx, MGetTranslate(pos_));

	//行列をモデルにセットする
	MV1SetMatrix(model_->GetModelHandle(), mtx);
}

void EnemyBase::SearchForPlayer(VECTOR playerPos)
{
	//敵からプレイヤーの直線距離
	distance_ = VSize(VSub(playerPos, pos_));

	//内積を取得する(返り値はコサイン)
	innerProduct = VDot(frontVector_, VNorm(VSub(playerPos, pos_)));

	//上記の結果から度数法に変える
	float aiu = acos(innerProduct);
	innerProduct = aiu / DX_PI_F * 180.0f;
}

//プレイヤーを突き飛ばす
void EnemyBase::ThrustAway(Player& player)
{
	VECTOR push = VScale(VNorm(frontVector_),110);

	VECTOR nockback = VAdd(player.GetStatus().pos, push);

	player.SetPos(nockback);
}

