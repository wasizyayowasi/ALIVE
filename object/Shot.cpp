#include "Shot.h"

#include "Player.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/SoundManager.h"

namespace {
	//消失ライン
	constexpr float vanishing_line = 1500.0f;

	//重力
	constexpr float gravity = -1.0f;
}

Shot::Shot(int handle,VECTOR initPos, VECTOR moveVec)
{
	model_ = std::make_shared<Model>(handle,Material::Stone);

	float scale = 10.0f;

	model_->SetScale(VGet(scale, scale, scale));

	pos_ = initPos;

	initialisePos_ = initPos;

	moveVec_ = moveVec;

	isEnable_ = true;
}

Shot::~Shot()
{
}

void Shot::Update()
{
	//ポジションの更新
	pos_ = VAdd(pos_, moveVec_);

	//モデルのポジションを設定
	model_->SetPos(pos_);

	//距離サイズ
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(initialisePos_, pos_);

	//距離サイズが一定ラインを超えると存在しているフラグをfalseにする
	if (distanceSize > vanishing_line) {
		isEnable_ = false;
	}

	//モデルの更新
	model_->Update();
}

void Shot::Draw()
{
	model_->Draw();
}

void Shot::HitCheck(Player& player)
{
	//プレイヤーの座標
	VECTOR playerPos = player.GetStatus().pos;

	//プレイヤーの高さ
	float playerHeight = player.GetStatus().height;

	//プレイヤーとショットの当たり判定
	//カプセルと球体
	bool hit = HitCheck_Sphere_Capsule(pos_, 50.0f, playerPos, VGet(playerPos.x, playerPos.y + playerHeight, playerPos.z),20.0f);

	//衝突したらノックバックベクトルを作り
	// 音を鳴らして
	//プレイヤーのベクトルに設定し、存在するフラグをfalseにする
	if (hit) {
		VECTOR nockBack = VScale(VNorm(moveVec_),5.0f);
		player.BulletHitMe(nockBack);

		SoundManager::GetInstance().Set3DSoundInfo(playerPos, 300.0f, "hit");
		SoundManager::GetInstance().PlaySE("hit");

		isEnable_ = false;
	}
}
