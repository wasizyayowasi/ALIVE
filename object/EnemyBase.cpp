#include "EnemyBase.h"
#include "Player.h"
#include "ShotManager.h"
#include "../util/Model.h"
#include "../util/ObjectManager.h"
#include "../util/Aster.h"

#include "../util/Util.h"

namespace {
	//モデルの初期回転ベクトル
	const VECTOR init_rot = { 0.0f,0.0f,-1.0f };

	//敵の視野角
	constexpr float viewing_angle = 30.0f;
	//敵がプレイヤーを視認できる範囲
	constexpr float visible_range = 500.0f;
	//敵のスピード
	constexpr float move_speed = 3.0f;

	//オブジェクト認知範囲
	constexpr float check_collition_radius = 200.0f;

	//敵モデルの高さ
	constexpr float model_height = 150.0f;

	//リーチ
	constexpr float within_reach = 80.0f;

	//右手のフレーム名
	const char* const hand_framename = "mixamorig:RightHandIndex2";
}


EnemyBase::EnemyBase(const char* fileName, LoadObjectInfo objInfo):CharacterBase(fileName,objInfo)
{
}

EnemyBase::EnemyBase(int handle, LoadObjectInfo objInfo) : CharacterBase(handle,objInfo)
{
	model_->SetAnimation(0, true, false);
	Aster_ = std::make_shared<Aster>(objInfo.pos);
	frontVec_ = init_rot;
}

void EnemyBase::Update(Player& player, const InputState& input)
{
	//プレイヤーの座標
	VECTOR playerPos = player.GetStatus().pos;

	//索敵
	if (!IsThereAnObject(playerPos)) {
		if (SearchForPlayer(playerPos)) {
			//プレイヤーを追跡する
			TrackingUpdate(playerPos);
		}
		else {
			model_->ChangeAnimation(0, true, true, 10);
		}
	}
	else{
		if (DistanceIsWithinRange()) {
			//目標マスの中心座標を取得
			Aster_->LocationInformation(playerPos, pos_);

			//経路探索
			RoutingUpdate(player);
		}
	}

	//モデルの更新
	model_->Update();

	if (distance_ < within_reach) {
		pushVec_ = VScale(VNorm(frontVec_), 10);
	}

	float pushVecSize = VSize(pushVec_);

	if (pushVecSize > 5.0f) {
		ThrustAway(player);
	}

}

void EnemyBase::Draw()
{
	model_->Draw();

//	Aster_->Draw();
}

void EnemyBase::HitColl(std::shared_ptr<ObjectBase> pointer)
{
}

void EnemyBase::TrackingUpdate(VECTOR playerPos)
{

	//プレイヤーと自分の差を算出し、正規化し、スピードを掛ける
	VECTOR distancePlayerAndEnemy = VSub(playerPos, pos_);
	distancePlayerAndEnemy.y = 0;
	VECTOR moveVec = VScale(VNorm(distancePlayerAndEnemy), move_speed);

	//回転行列の取得
	MATRIX rotMtx = MGetRotVec2(init_rot, distancePlayerAndEnemy);

	//拡縮行列の取得
	MATRIX scaleMtx = MGetScale(scale_);

	//正面ベクトルを取得する
	frontVec_ = VTransformSR(init_rot, rotMtx);

	//回転行列と拡縮行列の掛け算
	MATRIX mtx = MMult(rotMtx, scaleMtx);

	//ポジションの移動
	pos_ = VAdd(pos_, moveVec);

	//回転行列と拡縮行列を掛けた行列に平行移動行列を書ける
	mtx = MMult(mtx, MGetTranslate(pos_));

	//行列をモデルにセットする
	MV1SetMatrix(model_->GetModelHandle(), mtx);
}

bool EnemyBase::SearchForPlayer(VECTOR playerPos)
{
	//敵からプレイヤーの直線距離
	distance_ = VSize(VSub(playerPos, pos_));

	//内積を取得する(返り値はコサイン)
	innerProduct = VDot(frontVec_, VNorm(VSub(playerPos, pos_)));

	//上記の結果から度数法に変える
	float aiu = acos(innerProduct);
	innerProduct = aiu / DX_PI_F * 180.0f;

	if (!DistanceIsWithinRange()) {
		return false;
	}

	//敵の視野角よりも外側にいるまたは
	//敵からプレイヤーの距離が指定範囲より大きかったらreturn
	if (innerProduct > viewing_angle || distance_ > visible_range) {
		model_->ChangeAnimation(0, true, false, 20);
		return false;
	}

	return true;
}

//プレイヤーを突き飛ばす
void EnemyBase::ThrustAway(Player& player)
{
	pushVec_ = VScale(pushVec_, 0.96f);
	float size = VSize(pushVec_);
	VECTOR nockback = VAdd(player.GetStatus().pos,pushVec_);

	player.SetPos(nockback);
}

void EnemyBase::RoutingUpdate(Player& player)
{
	//プレイヤーの座標
	VECTOR playerPos = player.GetStatus().pos;

	VECTOR targetPos = Aster_->GetDestinationCoordinates(playerPos,pos_);

	//目標地点とポジションの距離を取得
	VECTOR distance = VSub(targetPos, pos_);

	float size = VSize(distance);


	//回転行列の取得
	MATRIX rotMtx = MGetRotVec2(init_rot, VSub(targetPos, pos_));

	//正面ベクトルを取得する
	frontVec_ = VTransformSR(init_rot, rotMtx);

	//拡縮行列の取得
	MATRIX scaleMtx = MGetScale(scale_);

	//回転行列と拡縮行列の掛け算
	MATRIX mtx = MMult(rotMtx, scaleMtx);
	//回転行列と拡縮行列を掛けた行列に平行移動行列を書ける
	mtx = MMult(mtx, MGetTranslate(pos_));
	//行列をモデルにセットする
	MV1SetMatrix(model_->GetModelHandle(), mtx);

	if (size > 5.0f) {
		//正規化
		VECTOR norm = VNorm(distance);
		//移動ベクトルを作成
		VECTOR moveVec = VScale(norm, move_speed);
		//移動
		pos_ = VAdd(pos_, moveVec);
		//ポジションの更新
		model_->SetPos(pos_);
	}
}

bool EnemyBase::IsThereAnObject(VECTOR playerPos)
{
	VECTOR distance = VSub(playerPos, pos_);
	int size = VSize(distance) / 50;
	VECTOR norm = VNorm(distance);
	
	bool noObject = false;

	for (int i = 0; i < size; i++) {
		VECTOR PointPos = VScale(norm, 50.0f * i);
		PointPos = VAdd(pos_, PointPos);
		noObject = Aster_->temp(PointPos);
		if (noObject) {
			break;
		}
	}

	return noObject;
}

bool EnemyBase::DistanceIsWithinRange()
{
	//プレイヤーと敵の座標差を見て、
	if (distance_ < visible_range) {
		model_->ChangeAnimation(1, true, false, 20);
		return true;
	}

	return false;
}

std::shared_ptr<Model> EnemyBase::AddCollModel()
{
	return nullptr;
}

void EnemyBase::Shot(std::shared_ptr<ShotManager>shotManager, VECTOR playerPos,float height)
{
	static int time = 0;

	VECTOR distance = VSub(playerPos, pos_);
	float distanceSize = VSize(distance);

	if (distanceSize > 600.0f) {
		if (++time % 60 == 0) {
			VECTOR framePos = model_->GetFrameLocalPosition(hand_framename);
			shotManager->Fire(framePos, playerPos, height);
		}
	}
}

