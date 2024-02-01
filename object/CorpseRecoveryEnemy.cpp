#include "CorpseRecoveryEnemy.h"

#include "../util/Model.h"
#include "../util/util.h"

namespace {
	//モデルの初期回転ベクトル
	const VECTOR init_rot = { 0.0f,0.0f,-1.0f };

	//敵がプレイヤーを視認できる範囲
	constexpr float visible_range = 500.0f;

	//敵の視野角
	constexpr float viewing_angle = 30.0f;

	//敵のスピード
	constexpr float move_speed = 3.0f;

	//右手のフレーム名
	const char* const hand_framename = "mixamorig:RightHandIndex2";
}

CorpseRecoveryEnemy::CorpseRecoveryEnemy(int handle,Material materialType, LoadObjectInfo objInfo):EnemyBase(handle, materialType, objInfo)
{
	initPos_ = objInfo.pos;
}

CorpseRecoveryEnemy::~CorpseRecoveryEnemy()
{
}

void CorpseRecoveryEnemy::Update(Player& player)
{

}

void CorpseRecoveryEnemy::Draw()
{
	model_->Draw();
}

void CorpseRecoveryEnemy::SearchForCorpse(VECTOR CorpsePos)
{
	//敵から死体の直線距離
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(CorpsePos, pos_);

	//内積を取得する(返り値はコサイン)
	innerProduct = VDot(frontVec_, VNorm(VSub(CorpsePos, pos_)));

	//上記の結果から度数法に変える
	float radian = acos(innerProduct);
	innerProduct = radian / DX_PI_F * 180.0f;

	//視野の範囲内かつ見える距離だったら検知したことにする
	if (innerProduct < viewing_angle) {
		if (distanceSize < visible_range) {
			isDetection_ = true;
		}
	}
}

void CorpseRecoveryEnemy::UpdateForCorpse(std::shared_ptr<ObjectBase> pointer)
{
	//エネミーのポジション
	VECTOR corpsePos = pointer->GetPos();

	SearchForCorpse(corpsePos);

	if (isDetection_) {
		VECTOR distance = VSub(corpsePos, pos_);
		VECTOR norm = VNorm(distance);
		VECTOR moveVec = VScale(moveVec, move_speed);

		pos_ = VAdd(pos_, moveVec);

		model_->SetPos(pos_);

		float distanceSize = VSize(distance);
		if (distanceSize < 3.0f) {
			VECTOR framePos = model_->GetFrameLocalPosition(hand_framename);
			pointer->SetPos(framePos);
			isTransit_ = true;
		}
	}

	if (isTransit_) {
		float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(initPos_, pos_);
		if (distanceSize < 3.0f) {
			pointer->SetPos(initPos_);
		}
	}
}
