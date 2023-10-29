#include "EnemyBase.h"
#include "Player.h"
#include "../util/Model.h"
#include "../util/ObjectManager.h"
#include "../util/Aster.h"


namespace {
	//追跡を止める範囲
	constexpr float range_to_stop_tracking = 60.0f;

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
	//rayの長さ
	constexpr float ray_radius = 200.0f;

	//敵モデルの高さ
	constexpr float model_height = 150.0f;
}


EnemyBase::EnemyBase(const char* fileName, LoadObjectInfo objInfo):CharacterBase(fileName,objInfo)
{
}

EnemyBase::EnemyBase(int handle, LoadObjectInfo objInfo) : CharacterBase(handle,objInfo)
{
	model_->SetAnimation(0, true, false);
	Aster_ = std::make_shared<Aster>();
}

void EnemyBase::Update(Player& player)
{
	//プレイヤーの座標
	VECTOR playerPos = player.GetStatus().pos;

	//索敵
	SearchForPlayer(playerPos);

	//モデルの更新
	model_->Update();

	if (aiueo(playerPos)) {
		//目標マスの中心座標を取得
		Aster_->LocationInformation(playerPos, pos_);

		//経路探索
		RoutingUpdate(player);
	}
	else {
		//プレイヤーを追跡する
		TrackingUpdate(playerPos);
	}


	if (distance_ < range_to_stop_tracking + 20.0f) {
		//ThrustAway(player);
	}

}

void EnemyBase::Draw()
{
	model_->Draw();

	Aster_->Draw();

	for (auto& point : debug_) {
		//DrawSphere3D(point.second, 16, 32, 0x0000ff, 0x0000ff, true);
	}

	DrawSphere3D(pos_, 16, 32, 0x0000ff, 0x0000ff, true);

	/*DrawLine3D(VGet(playerpos_.x, playerpos_.y + 20, playerpos_.z), VGet(pos_.x, pos_.y + 20, pos_.z), 0xffff00);



	VECTOR aiu = VSub(pos_, playerpos_);
	float ty = VSize(aiu);

	int asd = ty / 100;

	VECTOR temp = VSub(playerpos_, pos_);

	for (int i = 0; i < asd; i++) {
		aiu = VSub(playerpos_, pos_);
		aiu = VScale(VNorm(aiu), 100.0f + i * 100.0f);
		aiu = VAdd(pos_, aiu);

		DrawSphere3D(aiu, 16, 32, 0x0000ff, 0x0000ff, true);

		int x = temp.x / (100 * (asd - i));
		int z = temp.z / (100 * (asd - i));

		VECTOR pos = ConvWorldPosToScreenPos(aiu);
		DrawFormatString(pos.x, pos.y + 10, 0xff0000, "x:%d,z:%d", x,z);
	}*/
	

	/*VECTOR stomachPosition = VAdd(pos_,frontVec_);
	stomachPosition.y = stomachPosition.y + model_height / 2;

	VECTOR ray = VScale(frontVec_, ray_radius);
	ray = VAdd(pos_, ray);
	ray.y = ray.y += model_height / 2;
	DrawLine3D(stomachPosition,ray,0xff0000);
	if (isHit_) {
		DrawString(0, 64, "hit,hit,hit", 0x448844);
	}

	if (temp.HitNum > 0) {
		DrawTriangle3D(temp.Dim->Position[0], temp.Dim->Position[1], temp.Dim->Position[2], 0xff0000, true);
	}*/
}

void EnemyBase::TrackingUpdate(VECTOR playerPos)
{

	//プレイヤーと敵の座標差を見て、
	if (distance_ < range_to_stop_tracking) {
		model_->ChangeAnimation(0, true, false, 20);
		Aster_->Init();
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
	VECTOR distancePlayerAndEnemy = VSub(playerPos, pos_);
	VECTOR moveVec = VScale(VNorm(distancePlayerAndEnemy), move_speed);

	//回転行列の取得
	MATRIX rotMtx = MGetRotVec2(VGet(0, 0, -1), VSub(playerPos, pos_));

	//拡縮行列の取得
	MATRIX scaleMtx = MGetScale(scale_);

	//正面ベクトルを取得する
	frontVec_ = VTransformSR(VGet(0, 0, -1), rotMtx);

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
	innerProduct = VDot(frontVec_, VNorm(VSub(playerPos, pos_)));

	//上記の結果から度数法に変える
	float aiu = acos(innerProduct);
	innerProduct = aiu / DX_PI_F * 180.0f;
}

//プレイヤーを突き飛ばす
void EnemyBase::ThrustAway(Player& player)
{
	VECTOR push = VScale(VNorm(frontVec_),110);

	VECTOR nockback = VAdd(player.GetStatus().pos, push);

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

bool EnemyBase::aiueo(VECTOR playerPos)
{
	VECTOR distance = VSub(playerPos, pos_);
	int size = VSize(distance) / 100;
	VECTOR norm = VNorm(distance);
	
	bool noObject = false;

	for (int i = 0; i < size; i++) {
		VECTOR PointPos = VScale(norm, 100.0f * i);
		PointPos = VAdd(pos_, PointPos);
		noObject = Aster_->temp(PointPos);
		debug_[i] = PointPos;
		if (noObject) {
			break;
		}
	}

	return noObject;
}

