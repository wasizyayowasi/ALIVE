#include "ThrowEnemy.h"

#include "../object/Player.h"
#include "../object/ShotManager.h"

#include "../util/Util.h"
#include "../util/Model.h"

namespace {
	//敵がプレイヤーを視認できる範囲
	constexpr float visible_range = 1300.0f;

	//右手のフレーム名
	//const char* const hand_framename = "mixamorig:RightHandIndex2";
	const char* const hand_framename = "hand.R_end";

	//物を投げているときのアニメーションフレーム
	constexpr int throw_frame_time = 73;

	//敵の視野角
	constexpr float viewing_angle = 45.0f;
}

 ThrowEnemy::ThrowEnemy(int handle, Material materialType, LoadObjectInfo objInfo):EnemyBase(handle, materialType, objInfo)
{
	 //衝突判定の設定
	 isCollCheck_ = true;
	 model_->SetUseCollision(isCollCheck_, true);

	 //文字列の取得
	 std::string str = StrUtil::GetStringAfterSign(objInfo.name, "-");

	 //文字列がFakeだったら投げるふりをするフラグを立てる
	 if (str == "Fake") {
		 isFakeThrow_ = true;
	 }

	 //マテリアルの色を変える
	 MV1SetMaterialDifColor(model_->GetModelHandle(), 8, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
}

ThrowEnemy::~ThrowEnemy()
{
}

void ThrowEnemy::Update(Player& player)
{
	//プレイヤーの座標
	VECTOR playerPos = player.GetStatus().pos;

	//モデルの更新
	model_->Update();

	//プレイヤーを索敵する
	SearchForPlayer(playerPos);

	if (isDetection_) {
		//プレイヤーと敵の距離
		VECTOR distance = VNorm(VSub(playerPos, pos_));

		//Y軸は考慮しない
		distance.y = 0.0f;

		//回転行列と拡縮行列の合成
		MATRIX mtx = CombiningRotAndScallMat(distance);

		//回転行列と拡縮行列を掛けた行列に平行移動行列をかける
		mtx = MMult(mtx, MGetTranslate(pos_));

		//行列をモデルにセットする
		MV1SetMatrix(model_->GetModelHandle(), mtx);
	}
}

void ThrowEnemy::Draw()
{
	model_->Draw();

	//DrawLine3D(pos_, VScale(frontVec_, 800.0f), 0x0000ff);
}

void ThrowEnemy::SearchForPlayer(VECTOR playerPos)
{
	//敵からプレイヤーの直線距離
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(playerPos, pos_);

	float innerProduct = 0.0f;

	//内積を取得する(返り値はコサイン)
	innerProduct = VDot(VNorm(frontVec_), VNorm(VSub(playerPos, pos_)));

	//上記の結果から度数法に変える
	float radian = acos(innerProduct);
	innerProduct = radian / DX_PI_F * 180.0f;

	//視野の範囲内かつ距離が石を投げる距離よりも
	//短かったらプレイヤーを検知したことにする
	if (innerProduct < viewing_angle) {
		if (distanceSize < visible_range) {
			isDetection_ = true;
		}
		else {
			isDetection_ = false;
		}
	}
	else {
		isDetection_ = false;
	}
}

void ThrowEnemy::Shot(std::shared_ptr<ShotManager> shotManager, VECTOR playerPos, float height)
{
	//投げるアニメーションが終わったら
	//投げているという変数をfalseにする
	if (model_->IsAnimEnd()) {
		model_->ChangeAnimation(static_cast<int>(PlayerAnimType::Idle), false, false, 5);
		isThrow_ = false;
	}

	//プレイヤーを検知しているかどうか
	//検知していなかったらreturn
	if (!isDetection_) {
		return;
	}

	//プレイヤーのポジションとエネミーの距離のサイズを取得
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(playerPos, pos_);

	//投げている途中ではなかったら
	//アニメーションを投げるアニメーションに変更する
	if (!isThrow_) {
		//model_->ChangeAnimation(static_cast<int>(EnemyAnimType::Throw), false, false, 5);
		model_->ChangeAnimation(static_cast<int>(PlayerAnimType::Throw), false, false, 5);
		isThrow_ = true;
	}

	if (isFakeThrow_) {
		return;
	}

	//投げているアニメーションの特定フレームで
	//弾を発射する
	if (model_->GetSpecifiedAnimTime(throw_frame_time)) {
		VECTOR framePos = model_->GetFrameLocalPosition(hand_framename);
		shotManager->Fire(framePos, playerPos, height);
	}
}