#include "Player.h"

#include "../object/Corpse.h"
#include "../object/ObjectManager.h"

#include "../gimmick/Lever.h"
#include "../gimmick/ManualCrank.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"
#include "../util/ModelManager.h"
#include "../util/EffectManager.h"

#include <cmath>
#include <string>
#include <algorithm>

namespace {
	//重力
	constexpr float gravity = -0.4f;

	//フレームの名前
	const char* const frame_name = "hand.R_end";

	//プレイヤーの高さ
	constexpr float player_hegiht = 130.0f;

	//死体の数をカウントする最大数
	constexpr int max_death_count = 99;

	//初期正面ベクトル
	constexpr VECTOR front_vec = { 0,0,-1 };
}

//コンストラクタ
Player::Player(const LoadObjectInfo& info):updateFunc_(&Player::NormalUpdate),carryUpdateFunc_(&Player::CarryObject)
{
	//短縮化
	auto& model = ModelManager::GetInstance();
	auto& file = ExternalFile::GetInstance();

	//プレイヤー情報の初期化
	playerInfo_ = ExternalFile::GetInstance().GetPlayerInfo();

	//プレイヤーモデルの生成
	model_ = std::make_shared<Model>(model.GetModelHandle(objData_[static_cast<int>(ObjectType::Player)].name), Material::Other);

	//アニメーションの設定
	model_->SetAnimation(static_cast<int>(PlayerAnimType::Idle), true, false);

	//初期ポジションの取得
	VECTOR startPos = file.GetStartPos(file.GetStartName());

	//ポジションの設定
	model_->SetPos(startPos);

	//回転率の設定
	model_->SetRot(info.rot);

	//プレイヤーの大きさの調整
	model_->SetScale(info.scale);

	//コリジョンフレームの設定
	model_->SetUseCollision(true, true, "Character");

	//ポジションの初期化
	status_.pos = startPos;

	//拡縮率の初期化
	scale_ = info.scale;

	//ジャンプ情報の初期
	status_.jump.isJump = false;
	status_.jump.jumpVec = 0.0f;
	//プレイヤーの高さの設定
	status_.height = player_hegiht;

	tentativeRot_ = front_vec;
}

// デストラクタ
Player::~Player()
{
	ExternalFile::GetInstance().SetStartName("");
}

//更新
void Player::Update(const std::shared_ptr<ObjectManager>& objManager)
{
	//プレイヤーのアニメーション更新
	model_->Update();
	
	(this->*updateFunc_)(objManager);
}

//描画
void Player::Draw()
{
	//モデルの描画
	model_->Draw();

	//影もどきの描画
	DrawPolygon3D();
}

//弾に当たったらノックバックを追加する
void Player::BulletHitMe(const VECTOR& moveVec)
{
	//プレイヤーが死亡中だったらたまによるノックバックを無効にする
	if (updateFunc_ == &Player::DeathUpdate) 
	{
		return;
	}

	//ノックバックを追加する
	status_.moveVec = moveVec;

	//メンバ関数を変更する
	updateFunc_ = &Player::BulletHitMeUpdate;
}

//ポジションの設定
void Player::SetPos(const VECTOR& pos)
{
	//ステータスにポジションを代入する
	status_.pos = pos;

	//モデルにポジションを設定する
	model_->SetPos(status_.pos);
}

//ジャンプの設定
void Player::SetJumpInfo(bool isJump, float jumpVec)
{
	//ジャンプしているかのフラグを代入する
	status_.jump.isJump = isJump;

	//ジャンプベクトルを設定する
	status_.jump.jumpVec = jumpVec;

	//ポジションにジャンプベクトルを足す
	status_.pos.y += status_.jump.jumpVec;
}

//持ち運ぶ事が出来るフラグと持ち運ぶモデルのポインタを受け取る
void Player::SetCarryInfo(const bool isCarry, const std::shared_ptr<ObjectBase>& model)
{
	//持ち運び中フラグを設定する
	status_.situation.isCanBeCarried = isCarry;

	//死体のモデルポインタを取得
	corpseModelPointer_ = model;
}

//ManualCrankのポインタを設定する
void Player::SetCrankPointer(const std::shared_ptr<ManualCrank>& crank)
{
	//操作中フラグをtrueに設定する
	status_.situation.isGimmickCanBeOperated = true;

	//クランクのポインタを設定する
	crank_ = crank;
}

//レバーのポインタを設定する
void Player::SetLeverPointer(const std::shared_ptr<Lever>& lever)
{
	//操作中フラグをtrueに設定する
	status_.situation.isGimmickCanBeOperated = true;

	//レバーのポインタを設定する
	lever_ = lever;
}

//地面に描画する影の高さと踏んでいるオブジェクトの素材を設定する
void Player::SetRoundShadowHeightAndMaterial(const float height, const  Material materialType)
{
	//影の高さを設定する
	roundShadowHeight_ = height;

	//足元のオブジェクトの素材を設定する
	materialSteppedOn_ = materialType;
}

//HACK:↓汚い、気に食わない
//通常時の更新
void Player::NormalUpdate(const std::shared_ptr<ObjectManager>& objManager)
{
	//短縮化
	auto& input = InputState::GetInstance();

#ifdef _DEBUG
	//削除予定
	if (input.IsTriggered(InputType::Creative))
	{
		debugCreativeMode = !debugCreativeMode;
	}
#endif
	
	//持ち運び可能なオブジェクトに対してアクションを起こす
	if (input.IsTriggered(InputType::Activate))
	{
		if (status_.situation.isCanBeCarried) 
		{
			(this->*carryUpdateFunc_)();
		}
	}

	//操作を必要とするギミックに対してアクションを起こす
	if (input.IsTriggered(InputType::Activate))
	{
		//ベクトルを0にする
		status_.moveVec = VGet(0, 0, 0);

		//ギミックを操作出来るフラグがtrueならば
		//メンバ関数を変更する
		if (status_.situation.isGimmickCanBeOperated)
		{
			if (crank_ != nullptr)
			{
				//クランクを動かす準備をする
				updateFunc_ = &Player::GoCrankRotationPosition;
			}
			else if (lever_ != nullptr)
			{
				//レバーを動かす準備をする
				updateFunc_ = &Player::GoLeverPullPosition;
			}

			return;
		}
	}
	else {
		//クランクを操作しない場合ポインターをリセットする
		crank_.reset();

		//レバーを操作しない場合ポインターをリセットする
		lever_.reset();
	}

	status_.situation.isGimmickCanBeOperated = false;

	//死体を持ち運び中か
	if (status_.situation.isInTransit)
	{
		//プレイヤーの回転とともに死体のモデルも回転させる
		corpseModelPointer_->GetModelPointer()->SetRot(MathUtil::VECTORDegreeToRadian(status_.rot));

		//死体のポジションを常にプレイヤーの手の座標にする
		VECTOR framePos = model_->GetFrameLocalPosition(frame_name);
		corpseModelPointer_->GetModelPointer()->SetPos(framePos);
	}
	else
	{
		status_.situation.isCanBeCarried = false;
	}

	//アニメーションを待機にする
	ChangeAnimIdle();

	//移動
	MovingUpdate();

	//足音を鳴らす
	FootStepSound();

#ifdef _DEBUG
	//空中にいるとき
	//重力をベクトルに足してポジションに足す
	if (!debugCreativeMode)
	{
		if (status_.jump.isJump)
		{
			status_.jump.jumpVec += gravity;
			status_.pos.y += status_.jump.jumpVec;
			model_->SetPos(status_.pos);
		}
		else 
		{
			status_.jump.jumpVec = 0.0f;
		}
	}
#else
	//空中にいるとき
	//重力をベクトルに足してポジションに足す
	if (status_.jump.isJump) {
		status_.jump.jumpVec += gravity;
		status_.pos.y += status_.jump.jumpVec;
		model_->SetPos(status_.pos);
	}
	else {
		status_.jump.jumpVec = 0.0f;
	}
#endif // _DEBUG

	//持ち運び中だったら
	//以降の処理を行わない
	if (status_.situation.isInTransit)
	{
		return;
	}

#ifdef _DEBUG
	//メンバ関数ポインタをjumpUpdateに変更する
	if (!debugCreativeMode)
	{
		if (input.IsTriggered(InputType::Space))
		{
			if (!status_.jump.isJump)
			{
				SetJumpInfo(true,playerInfo_.jumpPower);
			}
			ChangeAnimNo(PlayerAnimType::Jump, false, 20);
			updateFunc_ = &Player::JumpUpdate;
			return;
		}
	}
	else {
		if (input.IsPressed(InputType::Space))
		{
			status_.moveVec.y = 10.0;
		}
	}
#else
	//メンバ関数ポインタをjumpUpdateに変更する
	if (input.IsTriggered(InputType::Space))
	{
		//ジャンプしているフラグがfalseだったら
		//ジャンプ処理を行う
		if (!status_.jump.isJump) {
			SetJumpInfo(true, playerInfo_.jumpPower);
		}

		//アニメーションをジャンプに変更する
		ChangeAnimNo(PlayerAnimType::Jump, false, 20);

		//メンバ関数を変更する
		updateFunc_ = &Player::JumpUpdate;
		return;
	}
#endif // _DEBUG

	//メンバ関数ポインタをDeathUpdateに変更する
	if (input.IsTriggered(InputType::Death))
	{
		//死体の数をカウントする
		deathCount_ = (std::min)(deathCount_ + 1, max_death_count);

		//移動ベクトルを0にする
		status_.moveVec = VGet(0, 0, 0);

		//メンバ関数を変更する
		updateFunc_ = &Player::DeathUpdate;
		return;
	}
}

//HACK:↓汚い、気に食わない
//移動の更新
void Player::MovingUpdate()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//移動スピードの取得
	float movingSpeed = Move();
	
	//tempRotationUpdate();
	
	//model_->SetPos(VAdd(status_.pos,status_.moveVec));

	//HACK：もっといいアニメーション番号変更があるはず
	if (movingSpeed != 0.0f) {
		if (movingSpeed > playerInfo_.walkSpeed) {
			//アニメーションの変更
			ChangeAnimNo(PlayerAnimType::Run, true, 20);
		}
		else if (movingSpeed <= playerInfo_.walkSpeed) {
			//アニメーションの変更
			ChangeAnimNo(PlayerAnimType::Walk, true, 20);
		}
		
	}

	if (VSize(status_.moveVec) == 0.0f) {
		status_.situation.isMoving = false;
		return;
	}

	//移動ベクトルを用意する
	status_.moveVec = VScale(VNorm(status_.moveVec), movingSpeed);
}

//移動処理
float Player::Move()
{
	//短縮化
	auto& input = InputState::GetInstance();

	//キーの押下をブール型に格納
	bool pressedUp = input.IsPressed(InputType::Up);
	bool pressedDown = input.IsPressed(InputType::Down);
	bool pressedLeft = input.IsPressed(InputType::Left);
	bool pressedRight = input.IsPressed(InputType::Right);
	bool pressedShift = input.IsPressed(InputType::Dush);

	status_.moveVec = VGet(0,0,0);

	status_.situation.isMoving = false;
	float movingSpeed = 0.0f;

	if (pressedUp || pressedDown || pressedLeft || pressedRight) {
		movingSpeed = PlayerSpeed(pressedShift);
		status_.situation.isMoving = true;
	}

	//HACK：汚い、リファクタリング必須
	if (pressedUp) {
		status_.moveVec.z += movingSpeed;
		targetAngle_ = 180.0f;
//		tentativeRot_ = { -0.01f,0,0.99f };
	}
	if (pressedDown) {
		status_.moveVec.z -= movingSpeed;
		targetAngle_ = 0.0f;
//		tentativeRot_ = { 0,0,-1 };
	}
	if (pressedLeft) {
		status_.moveVec.x -= movingSpeed;
		targetAngle_ = 90.0f;
//		tentativeRot_ = { -1,0,0 };
	}
	if (pressedRight) {
		status_.moveVec.x += movingSpeed;
		targetAngle_ = 270.0f;
//		tentativeRot_ = { 1,0,0 };
	}
	if (pressedUp && pressedRight) {
		targetAngle_ = 225.0f;
//		tentativeRot_ = { 1,0,1 };
	}
	if (pressedUp && pressedLeft) {
		targetAngle_ = 135.0f;
//		tentativeRot_ = { -1,0,1 };
	}
	if (pressedDown && pressedLeft) {
		targetAngle_ = 45.0f;
//		tentativeRot_ = { -1,0,-1 };
	}
	if (pressedDown && pressedRight) {
		targetAngle_ = 315.0f;
//		tentativeRot_ = { 1,0,-1 };
	}

	//回転処理
	RotationUpdate();

	VECTOR destination = VAdd(status_.pos, status_.moveVec);
	if (destination.z < -250.0f) {
		status_.moveVec.z = 0.0f;
	}

	return movingSpeed;

}

//回転の処理
void Player::RotationUpdate()
{
	//一回転の角度
	float oneRevolution = 360.0f;

	//目標の角度から現在の角度を引いて差を出している
	differenceAngle_ = targetAngle_ - angle_;

	//常にプレイヤーモデルを大周りさせたくないので
	//181度又は-181度以上の場合、逆回りにしてあげる
	if (differenceAngle_ >= oneRevolution / 2) {
		differenceAngle_ = targetAngle_ - angle_ - oneRevolution;
	}
	else if (differenceAngle_ <= -oneRevolution / 2) {
		differenceAngle_ = targetAngle_ - angle_ + oneRevolution;
	}

	//滑らかに回転させるため
	//現在の角度に回転スピードを増減させている
	if (differenceAngle_ < 0.0f) {
		status_.rot.y -= playerInfo_.rotSpeed;
		angle_ -= playerInfo_.rotSpeed;
	}
	else if (differenceAngle_ > 0.0f) {
		status_.rot.y += playerInfo_.rotSpeed;
		angle_ += playerInfo_.rotSpeed;
	}

	//360度、一周したら0度に戻すようにしている
	if (angle_ == oneRevolution || angle_ == -oneRevolution) {
		angle_ = 0.0f;
	}
	if (status_.rot.y == oneRevolution || status_.rot.y == -oneRevolution) {
		status_.rot.y = 0.0f;
	}

	//結果をモデルの回転情報として送る
	model_->SetRot(MathUtil::VECTORDegreeToRadian(status_.rot));
}

void Player::tempRotationUpdate()
{
	//行列
	MATRIX mtx = {};

	//回転行列の取得
	MATRIX rotMtx = MGetRotVec2(front_vec, VNorm(tentativeRot_));

	//拡縮行列の取得
	MATRIX scaleMtx = MGetScale(scale_);

	//平行移動行列
	MATRIX moveMtx = MGetTranslate(status_.pos);

	//回転行列と拡縮行列の乗算
	mtx = MMult(rotMtx, scaleMtx);

	//（回転行列と拡縮行列）と平行移動行列の乗算
	mtx = MMult(mtx, moveMtx);

	MV1SetMatrix(model_->GetModelHandle(),mtx);
}

//HACK:↓汚い、気に食わない
void Player::JumpUpdate(const std::shared_ptr<ObjectManager>& objManager)
{
	//プレイヤー移動関数
	Move();

	//ジャンプ処理
	{
		//ジャンプベクトルが0でジャンプ中ではなかったら
		//idle状態のアップデートに変更する、アニメーションも変更する
		if (!status_.jump.isJump) {
			updateFunc_ = &Player::NormalUpdate;
			return;
		}

		status_.jump.jumpVec += gravity;
		status_.pos.y += status_.jump.jumpVec;
		model_->SetPos(status_.pos);

	}
}

// プレイヤーの死体に与える情報を作る関数
void Player::DeathUpdate(const std::shared_ptr<ObjectManager>& objManager)
{
	//アニメーションの変更
	ChangeAnimNo(PlayerAnimType::Death, false, 20);

	if (model_->IsAnimEnd()) {
		CorpsePostProsessing(objManager);
	}
}

//死体の後処理
void Player::CorpsePostProsessing(const std::shared_ptr<ObjectManager>& objManager)
{
	//死体を生成する関数
	CorpseGenerater(objManager);			

	updateFunc_ = &Player::NormalUpdate;
}

// プレイヤーの死体をvector配列で管理する関数
void Player::CorpseGenerater(const std::shared_ptr<ObjectManager>& objManager)
{
	//配置データの作成
	LoadObjectInfo info = {};

	//モデルの中から指定のフレームの番号を取得する
	int frameNo = MV1SearchFrame(model_->GetModelHandle(), "PlaceToPutTheCorpse");

	//指定のフレームの座標を取得する
	VECTOR putPos = MV1GetFramePosition(model_->GetModelHandle(), frameNo);

	//取得した座標を死体のポジションとする
	info.pos = putPos;

	//プレイヤーの回転値を死体の回転値とする
	info.rot = MathUtil::VECTORDegreeToRadian(status_.rot);

	//拡縮率を同じにする
	info.scale = scale_;

	//死体を生成する
	objManager->CorpseGenerator(model_->GetModelHandle(),info, status_.animNo);
}

//荷物を運ぶ
void Player::CarryObject()
{
	
	if (!status_.situation.isCanBeCarried) return;

	status_.situation.isInTransit = true;

	corpseModelPointer_->SetIsTransit(true);

	carryUpdateFunc_ = &Player::DropOffObject;

}

//荷物をおろす
void Player::DropOffObject()
{
	if (status_.situation.isCanBeCarried) {
		status_.situation.isCanBeCarried = false;
		corpseModelPointer_->SetIsTransit(false);
		int frameNo = MV1SearchFrame(model_->GetModelHandle(), "PlaceToPutTheCorpse");
		VECTOR putPos = MV1GetFramePosition(model_->GetModelHandle(), frameNo);
		corpseModelPointer_->GetModelPointer()->SetPos(putPos);
		corpseModelPointer_.reset();
	}

	status_.situation.isInTransit = false;
	status_.situation.isCanBeCarried = false;

	carryUpdateFunc_ = &Player::CarryObject;

}

//クランクを回すためにクランクを回すポジションへと移動する
void Player::GoCrankRotationPosition(const std::shared_ptr<ObjectManager>& objManager)
{
	//クランクの立ってほしいポジションを取得する
	VECTOR standPos = crank_->GetStandingPosition();

	//立ってほしいポジションとプレイヤーの距離のサイズを取得する
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(status_.pos, standPos);

	//distanceSizeが一定の範囲外だったら
	//一定の速度で立ってほしいポジションに向かう
	if (distanceSize > 30.0f) {
		VECTOR distance = VNorm(VSub(standPos, status_.pos));
		VECTOR moveVec = VScale(distance, playerInfo_.walkSpeed);
		status_.pos = VAdd(status_.pos, moveVec);
		model_->SetPos(status_.pos);
	}
	//distanceSizeが一定の範囲内に入ったら
	//立ってほしいポジションをプレイヤーのポジションとする
	else {
		status_.pos = standPos;
		model_->SetPos(status_.pos);
		angle_ = -90.0f;
		status_.rot = VGet(0, angle_, 0);
		model_->SetRot(MathUtil::VECTORDegreeToRadian(status_.rot));
		ChangeAnimNo(PlayerAnimType::Crank, false, 20);
		updateFunc_ = &Player::CrankUpdate;
	}
}

//クランクを回転させるアップデート
void Player::CrankRotationUpdate(float rotZ) {

	float radian = MathUtil::DegreeToRadian(rotZ);

	int frameNo = MV1SearchFrame(crank_->GetModelPointer()->GetModelHandle(), "Crank");

	VECTOR pos = MV1GetFramePosition(crank_->GetModelPointer()->GetModelHandle(), frameNo);
	//クランクのポジションからオブジェクト全体のポジションを引いた距離
	VECTOR distance = { 0,1.8f,0 };

	MATRIX mat = {};

	float x = MathUtil::DegreeToRadian(90.0f);

	//平行移動行列
	MATRIX posMat = MGetTranslate(distance);
	//回転行列
	MATRIX rotMatZ = MGetRotZ(radian);
	MATRIX rotMatX = MGetRotX(x);

	mat = MMult(rotMatX, rotMatZ);
	mat = MMult(mat, posMat);

	MV1SetFrameUserLocalMatrix(crank_->GetModelPointer()->GetModelHandle(), frameNo, mat);

	crank_->SetRotZ(rotZ);
}

//クランクの更新
void Player::CrankUpdate(const std::shared_ptr<ObjectManager>& objManager)
{
	//短縮化
	auto& input = InputState::GetInstance();
	auto& sound = SoundManager::GetInstance();

	//移動ベクトルを0にする
	status_.moveVec = VGet(0, 0, 0);

	//クランクの回転を取得する(変化しているか確認用)
	float oldRotZ = crank_->GetRotZ();

	//クランクの回転を取得する(実際に値を変えるよう)
	float rotZ = crank_->GetRotZ();

	if (input.IsPressed(InputType::Down))
	{
		rotZ = (std::max)(rotZ - 3.0f, crank_->GetMaxRotZ());
		CrankRotationUpdate(rotZ);
	}
	else if (input.IsPressed(InputType::Up))
	{
		rotZ = (std::min)(rotZ + 3.0f, 0.0f);
		CrankRotationUpdate(rotZ);
	}

	if (oldRotZ != rotZ && !sound.CheckSoundFile("crank"))
	{
		sound.Set3DSoundInfo(crank_->GetStandingPosition(), 1500.0f, "crank");
		sound.PlaySE("crank");
	}

	int naturalNumber = static_cast<int>((std::max)(rotZ, -rotZ));
	float animTime = static_cast<float>(naturalNumber % 360) / 3;

	model_->SetAnimationFrame(animTime);

	if (input.IsTriggered(InputType::Activate))
	{
		status_.situation.isGimmickCanBeOperated = false;
		crank_.reset();
		updateFunc_ = &Player::NormalUpdate;
	}
}

//レバーを倒すポジションへ行く
void Player::GoLeverPullPosition(const std::shared_ptr<ObjectManager>& objManager)
{
	//クランクの立ってほしいポジションを取得する
	VECTOR standPos = lever_->GetStandingPosition();
	//立ってほしいポジションとプレイヤーの距離のサイズを取得する
	float distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(status_.pos, standPos);

	//distanceSizeが一定の範囲外だったら
	//一定の速度で立ってほしいポジションに向かう
	if (distanceSize > 30.0f)
	{
		VECTOR distance = VNorm(VSub(standPos, status_.pos));
		VECTOR moveVec = VScale(distance, playerInfo_.walkSpeed);
		status_.pos = VAdd(status_.pos, moveVec);
		model_->SetPos(status_.pos);
	}
	//distanceSizeが一定の範囲内に入ったら
	//立ってほしいポジションをプレイヤーのポジションとする
	else
	{
		status_.pos = standPos;
		model_->SetPos(status_.pos);
		angle_ = 0.0f;
		lever_->OnAnimation();
		status_.rot = VGet(0, angle_, 0);
		model_->SetRot(MathUtil::VECTORDegreeToRadian(status_.rot));
		ChangeAnimNo(PlayerAnimType::LeverOn, false, 10);
		updateFunc_ = &Player::LeverUpdate;
	}
}

//レバーの更新
void Player::LeverUpdate(const std::shared_ptr<ObjectManager>& objManager)
{
	if (!lever_->IsOn())
	{
		updateFunc_ = &Player::NormalUpdate;
	}
}

//投擲物との衝突アップデート
void Player::BulletHitMeUpdate(const std::shared_ptr<ObjectManager>& objManager)
{
	//重力
	status_.jump.jumpVec += gravity;
	status_.moveVec.y = status_.jump.jumpVec;

	//ノックバック
	status_.moveVec = VScale(status_.moveVec, 0.96f);

	//移動ベクトルを足した行き先を取得する
	VECTOR destinationPos = VAdd(status_.pos, status_.moveVec);

	//移動ベクトルを足した行き先が-250以下だったら
	//移動ベクトルのZ値を0にする
	if (destinationPos.z < -250.0f)
	{
		status_.moveVec.z = 0.0f;
	}

	//プレイヤーのポジションに移動ベクトルを足す
	status_.pos = VAdd(status_.pos, status_.moveVec);

	//モデルにポジションを設定する
	model_->SetPos(status_.pos);


	//以下、不安要素
	float moveVecSize = VSize(status_.moveVec);

	if (moveVecSize < 2.0f)
	{
		updateFunc_ = &Player::NormalUpdate;
	}
}

//アニメーションを待機に変更する
void Player::ChangeAnimIdle()
{
	//待機アニメーションに戻す
	if (!status_.situation.isMoving)
	{
		ChangeAnimNo(PlayerAnimType::Idle, true, 20);
	}
}

//アニメーションの変更を行う
void Player::ChangeAnimNo(PlayerAnimType type, bool isAnimLoop, int changeTime)
{
	status_.animNo = static_cast<int>(type);
	status_.isAnimLoop = isAnimLoop;
	model_->ChangeAnimation(status_.animNo, status_.isAnimLoop, false, changeTime);
}

//プレイヤーの速度設定
float Player::PlayerSpeed(bool pressedShift)
{
	//シフトが押されているかどうか
	if (pressedShift)
	{
#ifdef _DEBUG
		if (debugCreativeMode) {
			return playerInfo_.runningSpeed * 3;
		}
		return playerInfo_.runningSpeed;
#else
		return playerInfo_.runningSpeed;
#endif // _DEBUG
	} 
	
	return playerInfo_.walkSpeed;
}

//足音の再生
void Player::FootStepSound()
{
	//音が鳴っているか
	bool playSound = false;

	//プレイヤーのアニメーションによって
	//アニメーションフレームの特定フレームで
	//音が鳴っているかのフラグをtrueにする
	switch (static_cast<PlayerAnimType>(status_.animNo))
	{
	case PlayerAnimType::Walk:

		if (model_->GetSpecifiedAnimTime(80) || model_->GetSpecifiedAnimTime(160)) {
			playSound = true;
		}

		break;
	case PlayerAnimType::Run:

		if (model_->GetSpecifiedAnimTime(6) || model_->GetSpecifiedAnimTime(45)) {
			playSound = true;
		}

		break;
	}

	//音が鳴っていない場合リターン
	if (!playSound)
	{
		return;
	}

	//プレイヤーの足元のオブジェクトの素材によって
	//鳴らす音を変える
	switch (materialSteppedOn_)
	{
	case Material::Iron:
		SoundManager::GetInstance().Set3DSoundInfo(status_.pos, 1500.0f, "ironStep");
		SoundManager::GetInstance().PlaySE("ironStep");
		break;
	case Material::Stone:
		SoundManager::GetInstance().Set3DSoundInfo(status_.pos, 1500.0f, "asphaltStep");
		SoundManager::GetInstance().PlaySE("asphaltStep");
		break;
	}
}

//落ち影を作成、描画
void Player::DrawPolygon3D()
{
	//頂点の数分配列を作る
	VERTEX3D vertex[7] = {};
	//三角形を作成する順番を保存する配列
	WORD index[18] = {};

	//カラー
	COLOR_U8 difColor = GetColorU8(51, 51, 51, 125);
	COLOR_U8 spcColor = GetColorU8(0, 0, 0, 0);

	//法線ベクトル
	VECTOR norm = VGet(0.0f, 1.0f, 0.0f);

	//角度に寄って頂点の位置を変更する
	float angle = 0.0f;

	//六角形の中心座標を取得
	vertex[0].pos = VGet(status_.pos.x, roundShadowHeight_, status_.pos.z);
	vertex[0].norm = norm;
	vertex[0].dif = difColor;
	vertex[0].spc = GetColorU8(0, 0, 0, 0);
	vertex[0].u = 0.0f;
	vertex[0].v = 0.0f;
	vertex[0].su = 0.0f;
	vertex[0].sv = 0.0f;

	//角度ごとの頂点を取得
	for (int i = 1; i < 7; i++)
	{
		vertex[i].pos = VertexPosition(angle);
		vertex[i].norm = norm;
		vertex[i].dif = difColor;
		vertex[i].spc = spcColor;
		vertex[i].u = 0.0f;
		vertex[i].v = 0.0f;
		vertex[i].su = 0.0f;
		vertex[i].sv = 0.0f;
		angle += 60.0f;
	}

	//三角形を作成する順番
	index[0] = 0;
	index[1] = 1;
	index[2] = 2;
	index[3] = 0;
	index[4] = 2;
	index[5] = 3;
	index[6] = 0;
	index[7] = 3;
	index[8] = 4;
	index[9] = 0;
	index[10] = 4;
	index[11] = 5;
	index[12] = 0;
	index[13] = 5;
	index[14] = 6;
	index[15] = 0;
	index[16] = 6;
	index[17] = 1;

	DrawPolygonIndexed3D(vertex, 7, index, 6, DX_NONE_GRAPH, true);
}

//プレイヤーの落ち影に使用する頂点を取得
VECTOR Player::VertexPosition(float angle)
{

	VECTOR pos = {};

	//度数法を弧度法に変換する
	float radian = MathUtil::DegreeToRadian(angle);
	
	//角度による座標を取得する
	pos.x = sin(radian);
	pos.z = cos(radian);
	pos.y = 0.0f;

	//ポジションを25倍する(サイズ調整)
	pos = VScale(pos, 25.0f);

	//プレイヤーのポジションと上記で取得したポジションを足す
	pos = VAdd(status_.pos, pos);

	//Y座標は線とポリゴンの当たり判定で取得した
	//一番近いポリゴンのY座標を代入する
	pos.y = roundShadowHeight_;

	return pos;
}
