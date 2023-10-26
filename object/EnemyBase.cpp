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
//	SearchForPlayer(playerPos);

	//モデルの更新
	model_->Update();

	//AvoidAndTrackObjectsUpdate(playerPos);

	//プレイヤーを追跡する
//	TrackingUpdate(playerPos);

	VECTOR pos = Aster_->LocationInformation(playerPos,pos_);
	tempa = pos;
	tempdistance = VSub(pos_,pos);
	VECTOR moveVec = VScale(VNorm(tempdistance), move_speed);
//	pos_ = VAdd(pos_,moveVec);

	model_->SetPos(pos_);

	if (distance_ < range_to_stop_tracking + 20.0f) {
		//ThrustAway(player);
	}

}

void EnemyBase::Draw()
{
	model_->Draw();

	Aster_->Draw();

	DrawSphere3D(pos_, 8, 32, 0xff0000, 0xff0000, true);
	DrawSphere3D(tempdistance, 32, 32, 0xff0000, 0xff0000, true);

	DrawFormatString(0, 64, 0x448844, "%.2f,%.2f,%.2f", tempdistance.x, tempdistance.y, tempdistance.z);
	DrawFormatString(0, 80, 0x448844, "%.2f,%.2f,%.2f", tempa.x, tempa.y, tempa.z);

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

//void EnemyBase::AvoidAndTrackObjectsUpdate(VECTOR playerPos)
//{
//	VECTOR distancePlayerAndEnemy = VSub(playerPos, pos_);
//	VECTOR moveVec = VScale(VNorm(distancePlayerAndEnemy), move_speed);
//
//	VECTOR destination = VAdd(frontVec_, moveVec);
//
//	for (auto& model : ObjectManager::GetInstance().GetAllCheckCollModel()) {
//		//衝突判定の更新
//		MV1RefreshCollInfo(model->GetModelHandle(), model->GetColFrameIndex());
//
//		//敵座標からの検出範囲
//		auto rangeOfDetection = check_collition_radius + VSize(moveVec);
//		//衝突判定の結果
//		auto result = MV1CollCheck_Sphere(model->GetModelHandle(), model->GetColFrameIndex(), pos_, rangeOfDetection);
//		//listに追加
//		hitDim_.push_back(result);
//	}
//
//	//moveVecの絶対値で動いたか動いてないかを判断
//	if (fabs(moveVec.x) > 0.000001f || fabs(moveVec.z) > 0.000001f) {
//		isMove_ = true;
//	}
//	else {
//		isMove_ = false;
//	}
//
//	CheckHeadOnCollision();
//
//	for (auto& hit : hitDim_) {
//		MV1CollResultPolyDimTerminate(hit);
//	}
//
//	temp = hitDim_.front();
//
//	hitDim_.clear();
//
//}
//
//void EnemyBase::CheckHeadOnCollision()
//{
//	//大体の腹の高さを得る
//	VECTOR stomachPosition = VAdd(pos_, frontVec_);
//	stomachPosition.y = stomachPosition.y + model_height / 2;
//
//	VECTOR ray = VScale(frontVec_, ray_radius);
//	ray = VAdd(pos_, ray);
//	ray.y = ray.y += model_height / 2;
//
//	for (auto& dim : hitDim_) {
//		//dim.HitNum
//		auto result = HitCheck_Line_Triangle(stomachPosition, ray, dim.Dim->Position[0], dim.Dim->Position[1], dim.Dim->Position[2]);
//		if (result.HitFlag) {
//			isHit_ = result.HitFlag;
//			break;
//		}
//		isHit_ = result.HitFlag;
//	}
//	
//
//}

