#include "Player.h"
#include "../util/InputState.h"
#include "../util/Model.h"
#include "../util/CheckCollisionModel.h"
#include "../util/LoadExternalFile.h"
#include "../util/SoundManager.h"
#include "../util/ObjectManager.h"
#include<algorithm>
#include<string>

namespace {
	//重力
	constexpr float gravity = -0.4f;

	//ファイルパス
	const char* const player_Filename = "DATA/player/player15.mv1";
	//モデルフレーム名
	const char* const coll_frame_death = "CollisionDeath";
	const char* const coll_frame_Sit = "CollisionSit";
	const char* const coll_frame_Stand = "CollisionStand";

	//プレイヤーサイズ
	const VECTOR player_scale = { 0.5f,0.5f ,0.5f };

	//初期プレイヤーの回転角度
	const VECTOR start_player_rot = { 0.0f,0.0f ,0.0f };

	//プレイヤーの高さ
	constexpr float player_hegiht = 150.0f;

}

using namespace std;

Player::Player(const char* const filename):updateFunc_(&Player::IdleUpdate)
{

	//ジャンプ情報の初期
	status_.jump.isJump = false;
	status_.jump.jumpVec = 0.0f;
	//プレイヤーの高さの設定
	status_.height = player_hegiht;
}

Player::Player(int handle) :updateFunc_(&Player::IdleUpdate)
{
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
}


void Player::Init()
{

	auto loadExternalFile = LoadExternalFile::GetInstance(isContinue_);
	//プレイヤー情報の初期化
	{
		playerInfo_.jumpPower = loadExternalFile.GetPlayerInfo().jumpPower;
		playerInfo_.runningJumpPower = loadExternalFile.GetPlayerInfo().runningJumpPower;
		playerInfo_.walkSpeed = loadExternalFile.GetPlayerInfo().walkSpeed;
		playerInfo_.runningSpeed = loadExternalFile.GetPlayerInfo().runningSpeed;
		playerInfo_.rotSpeed = loadExternalFile.GetPlayerInfo().rotSpeed;
		for (int i = 0; i < static_cast<int>(AnimType::max); i++) {
			playerInfo_.anim_[i] = loadExternalFile.GetPlayerInfo().animNo_[i];
		}
	}

	for (int i = 0; i < static_cast<int>(AnimType::max); i++)
	{
		animType_[static_cast<AnimType>(i)] = playerInfo_.anim_[i];
	}

	//プレイヤーモデルの生成
	PModel_ = make_shared<Model>(player_Filename);
	//アニメーションの設定
	PModel_->SetAnimation(animType_[AnimType::idle], true, false);
	//プレイヤーの大きさの調整
	PModel_->SetScale(player_scale);
	//マップやブロックなどの当たり判定の生成
	checkCollisionModel_ = make_shared<CheckCollisionModel>();
	//コリジョンフレームの設定
	PModel_->SetCollFrame("Character");
}



void Player::Update(const InputState& input)
{
	//移動ベクトルのリセット
	status_.moveVec = { 0.0f,0.0f,0.0f };

	//プレイヤーのアニメーション更新
	PModel_->Update();
	
	(this->*updateFunc_)(input);

	//プレイヤーとその他オブジェクトとの衝突判定
	checkCollisionModel_->CheckCollision(*this, status_.moveVec,player_hegiht, status_.jump.isJump, status_.jump.jumpVec);
}


void Player::Draw()
{
	PModel_->Draw();

	DrawFormatString(0, 48, 0x448844, "%.2f", tempAngle_);
	DrawFormatString(0, 64, 0x448844, "%d", deathCount_);

}

VECTOR Player::GetRot()
{
	return { status_.rot.x, status_.rot.y * DX_PI_F / 180.0f, status_.rot.z};
}


void Player::SetPos(VECTOR pos)
{
	status_.pos = pos;
	PModel_->SetPos(status_.pos);
}


void Player::SetJumpInfo(bool isJump, float jumpVec)
{
	status_.jump.isJump = isJump;
	status_.jump.jumpVec = jumpVec;
}


//HACK:↓汚い、気に食わない
/// <summary>
/// アニメーションがidle状態の時に行われる
/// </summary>
/// <param name="input">外部装置の入力情報を参照する</param>
void Player::IdleUpdate(const InputState& input)
{

	//メンバ関数ポインタをsitUpdateに変更する
	if (input.IsTriggered(InputType::ctrl)) {
		updateFunc_ = &Player::IdleToSitup;
		return;
	}

	//メンバ関数ポインタをrunningJumpUpdate、
	//jumpUpdateのどちらかに変更する
	if (input.IsTriggered(InputType::space)) {
		if (isClim_) {
			updateFunc_ = &Player::ClimUpdate;
			return;
		}
		else if (input.IsPressed(InputType::shift)) {
			updateFunc_ = &Player::RunningJumpUpdate;
			return;
		}
		else {
			updateFunc_ = &Player::JumpUpdate;
			return;
		}
	}

	if (input.IsTriggered(InputType::death)) {
		updateFunc_ = &Player::DeathUpdate;
		return;
	}

	ChangeAnimIdle();
	MovingUpdate(input);

	
	CarryObjectUpdate(input);
	

	//TODO：↓なくしたい
	{
		if (status_.jump.isJump && status_.jump.jumpVec == 0.0f) {
			status_.pos.y += gravity * 20;
		}

		if (status_.pos.y <= -400.0f) {
			status_.pos = checkPoint_;
		}
		if (input.IsTriggered(InputType::space)) {
			status_.pos = checkPoint_;
		}
	}
	
}

/// <summary>
/// アニメーションをidleに戻す関数
/// </summary>
void Player::ChangeAnimIdle()
{
	//待機アニメーションに戻す
	if (!isMoving_) {

		if (status_.animNo == animType_[AnimType::carryWalking]) {
			status_.animNo == animType_[AnimType::carry];
		}
		else {
			status_.animNo = animType_[AnimType::idle];
		}
		status_.isAnimLoop = true;
		PModel_->ChangeAnimation(status_.animNo, status_.isAnimLoop, false, 20);
	}
}

//HACK:↓汚い、気に食わない
/// <summary>
/// プレイヤーを移動させるための関数
/// </summary>
/// <param name="input">外部装置の入力情報を参照する</param>
void Player::MovingUpdate(const InputState& input)
{
	//キーの押下をブール型に格納
	bool pressedUp = input.IsPressed(InputType::up);
	bool pressedDown = input.IsPressed(InputType::down);
	bool pressedLeft = input.IsPressed(InputType::left);
	bool pressedRight = input.IsPressed(InputType::right);
	bool pressedShift = input.IsPressed(InputType::shift);

	isMoving_ = false;
	float movingSpeed = 0.0f;

	if (pressedUp || pressedDown || pressedLeft || pressedRight) {
		movingSpeed = PlayerSpeed(pressedShift);
		isMoving_ = true;
	}

	//改善しよう
	{
		//HACK：汚い、リファクタリング必須
		if (pressedUp) {
			status_.moveVec.z += movingSpeed;
			targetAngle_ = 180.0f;
		}
		if (pressedDown) {
			status_.moveVec.z -= movingSpeed;
			targetAngle_ = 0.0f;
		}
		if (pressedLeft) {
			status_.moveVec.x -= movingSpeed;
			targetAngle_ = 90.0f;
		}
		if (pressedRight) {
			status_.moveVec.x += movingSpeed;
			targetAngle_ = 270.0f;
		}
		if (pressedUp && pressedRight) {
			targetAngle_ = 225.0f;
		}
		if (pressedUp && pressedLeft) {
			targetAngle_ = 135.0f;
		}
		if (pressedDown && pressedLeft) {
			targetAngle_ = 45.0f;
		}
		if (pressedDown && pressedRight) {
			targetAngle_ = 315.0f;
		}

		//HACK：もっといいアニメーション番号変更があるはず
		if (status_.animNo != animType_[AnimType::runningJump] && status_.animNo != animType_[AnimType::jump] && status_.animNo != animType_[AnimType::carryWalking]) {
			if (movingSpeed != 0.0f) {
				if (movingSpeed > playerInfo_.walkSpeed) {
					status_.animNo = animType_[AnimType::run];
					status_.isAnimLoop = true;
				}
				else if (movingSpeed <= playerInfo_.walkSpeed) {
					status_.animNo = animType_[AnimType::walk];
					status_.isAnimLoop = true;
				}
			}
		}

		//回転処理
		RotationUpdate();

		if (VSize(status_.moveVec) == 0.0f) {
			isMoving_ = false;
			return;
		}
		//移動ベクトルを用意する
		status_.moveVec = VScale(VNorm(status_.moveVec), movingSpeed);

		//アニメーションの変更
		PModel_->ChangeAnimation(status_.animNo, status_.isAnimLoop, false, 20);


		//デバッグ用
		/*{
			if (input.isPressed(InputType::next)) {
				pos_.y += movingSpeed_;
			}
			if (input.isPressed(InputType::prev)) {
				pos_.y -= movingSpeed_;
			}
		}*/
	}
}

//完成品だから今後いじらなくていいと思う
/// <summary>
/// プレイヤーの回転処理を行う関数
/// </summary>
void Player::RotationUpdate()
{
	//目標の角度から現在の角度を引いて差を出している
	differenceAngle_ = targetAngle_ - tempAngle_;

	//常にプレイヤーモデルを大周りさせたくないので
	//181度又は-181度以上の場合、逆回りにしてあげる
	if (differenceAngle_ >= 180.0f) {
		differenceAngle_ = targetAngle_ - tempAngle_ - 360.0f;
	}
	else if (differenceAngle_ <= -180.0f) {
		differenceAngle_ = targetAngle_ - tempAngle_ + 360.0f;
	}

	//滑らかに回転させるため
	//現在の角度に回転スピードを増減させている
	if (differenceAngle_ < 0.0f) {
		status_.rot.y -= playerInfo_.rotSpeed;
		tempAngle_ -= playerInfo_.rotSpeed;
	}
	else if (differenceAngle_ > 0.0f) {
		status_.rot.y += playerInfo_.rotSpeed;
		tempAngle_ += playerInfo_.rotSpeed;
	}

	//360度、一周したら0度に戻すようにしている
	if (tempAngle_ == 360.0f || tempAngle_ == -360.0f) {
		tempAngle_ = 0.0f;
	}
	if (status_.rot.y == 360.0f || status_.rot.y == -360.0f) {
		status_.rot.y = 0.0f;
	}

	//結果をモデルの回転情報として送る
	PModel_->SetRot({ status_.rot.x,status_.rot.y * DX_PI_F / 180.0f,status_.rot.z });
}

//オブジェクトを登る
void Player::ClimUpdate(const InputState& input)
{
	status_.animNo = animType_[AnimType::clim];
	status_.isAnimLoop = false;

	PModel_->ChangeAnimation(status_.animNo, status_.isAnimLoop, false, 20);

	VECTOR localPosition;

	if (PModel_->IsAnimEnd()) {
		status_.pos = FramPosition("mixamorig:LeftToeBase", "mixamorig:RightToeBase");
		PModel_->SetPos(status_.pos);

		status_.animNo = animType_[AnimType::stand];
		status_.isAnimLoop = false;
		PModel_->SetAnimation(status_.animNo, status_.isAnimLoop, true);
		updateFunc_ = &Player::StandUpdate;
	}

}

//HACK:↓汚い、気に食わない
/// <summary>
/// 走りジャンプではないときのジャンプ
/// </summary>
/// <param name="input">外部装置の入力情報を参照する</param>
void Player::JumpUpdate(const InputState& input)
{
	//プレイヤー移動関数
	MovingUpdate(input);

	//ジャンプ処理
	{
		//アニメーション変更と脚力をジャンプベクトルに足す
		if (!status_.jump.isJump && status_.animNo != animType_[AnimType::jump]) {
			status_.animNo = animType_[AnimType::jump];
			status_.jump.jumpVec += playerInfo_.jumpPower;
			status_.jump.isJump = true;
			status_.isAnimLoop = false;
			PModel_->ChangeAnimation(status_.animNo, status_.isAnimLoop, false, 20);
		}

		//空中にいるとき
		//重力をベクトルに足してポジションに足す
		if (status_.jump.isJump) {
			status_.jump.jumpVec += gravity;
			status_.pos.y += status_.jump.jumpVec;
		}

		//ジャンプベクトルが0でジャンプ中ではなかったら
		//idle状態のアップデートに変更する、アニメーションも変更する
		if (status_.jump.jumpVec == 0.0f && !status_.jump.isJump) {
			updateFunc_ = &Player::IdleUpdate;
			status_.animNo = animType_[AnimType::idle];
			return;
		}

	}
}


//HACK:↓汚い、気に食わない
/// <summary>
/// プレイヤーが走っているときのジャンプ
/// </summary>
/// <param name="input">外部装置の入力情報を参照する</param>
void Player::RunningJumpUpdate(const InputState& input)
{
	//プレイヤー移動関数
	MovingUpdate(input);

	//アニメーション変更と脚力をジャンプベクトルに足す
	if (!status_.jump.isJump && status_.animNo != animType_[AnimType::runningJump]) {
		status_.animNo = animType_[AnimType::runningJump];
		status_.jump.jumpVec += playerInfo_.runningJumpPower;
		status_.isAnimLoop = false;
		status_.jump.isJump = true;
		PModel_->ChangeAnimation(status_.animNo, status_.isAnimLoop, false, 20);
	}

	//HACK：変数が仮のまま　+　どうするか悩んでいる
	//アニメーションの総時間によって、重力を変更する
	temp = PModel_->GetAnimTotalTime() + 2;
	tempGravity = -(playerInfo_.runningJumpPower * 2 / temp);

	//空中にいるとき
	//重力をベクトルに足してポジションに足す
	if (status_.jump.isJump) {
		status_.jump.jumpVec += tempGravity;
		status_.pos.y += status_.jump.jumpVec;
	}

	//ジャンプベクトルが0でジャンプ中ではなかったら
	//idle状態のアップデートに変更する、アニメーションも変更する
	if (status_.jump.jumpVec == 0.0f && !status_.jump.isJump) {
		updateFunc_ = &Player::IdleUpdate;
		status_.animNo = animType_[AnimType::run];
		status_.isAnimLoop = true;
		return;
	}
}

/// <summary>
/// プレイヤーの死体に与える情報を作る関数
/// </summary>
/// <param name="input">外部装置の入力情報を参照する</param>
void Player::DeathUpdate(const InputState& input)
{
	deathPos = status_.pos;				//死んだ場所を残す

	status_.isAnimLoop = false;			//アニメーションのループをするか

	//座るアニメーション以外だったら死ぬアニメーションに変える
	if (status_.animNo != animType_[AnimType::sit]) {
		status_.animNo = animType_[AnimType::death];
		PModel_->ChangeAnimation(status_.animNo, status_.isAnimLoop, false, 20);
	}

	if (PModel_->IsAnimEnd()) {
		deathCount_++;					//死亡回数をカウントする
		DeathPersonPostProsessing();
	}

}

//死体の後処理
void Player::DeathPersonPostProsessing()
{
	status_.pos = checkPoint_;				//チェックポイントにプレイヤーを帰す

	DeadPersonGenerater();			//死体を生成する関数

	updateFunc_ = &Player::IdleUpdate;
}

/// <summary>
/// プレイヤーの死体をvector配列で管理する関数
/// </summary>
void Player::DeadPersonGenerater()
{
	auto& objManager = ObjectManager::GetInstance();

	VECTOR rot = { status_.rot.x,status_.rot.y * DX_PI_F / 180.0f,status_.rot.z };

	objManager.DeadPersonGenerator(ObjectType::deadPerson, PModel_->GetModelHandle(), deathPos, rot, status_.animNo);
}

/// <summary>
/// プレイヤーに座るアニメーションをさせる関数
/// </summary>
/// <param name="input">外部装置の入力情報を参照する</param>
void Player::SitUpdate(const InputState& input)
{
	//立ち上がるためのコマンド
	if (input.IsTriggered(InputType::ctrl)) {
		status_.animNo = animType_[AnimType::situpToIdle];
		status_.isAnimLoop = false;
		PModel_->ChangeAnimation(status_.animNo, status_.isAnimLoop, false, 20);
	}
	
	//立つ家庭のアニメーションが終わったらidleupdateに変更する
	if (status_.animNo == animType_[AnimType::situpToIdle] && PModel_->IsAnimEnd()) {
		updateFunc_ = &Player::IdleUpdate;
		isSitting_ = false;
		return;
	}

	if (status_.animNo == animType_[AnimType::situpToIdle]) return;

	//死ぬコマンド
	if (input.IsTriggered(InputType::death)) {
		DeathUpdate(input);
		isSitting_ = false;
		updateFunc_ = &Player::IdleUpdate;
		return;
	}

}

void Player::IdleToSitup(const InputState& input)
{
	//アニメーションを座る過程のアニメーションに変更
	//座っているフラグを立て、アニメーションループ変数を折る
	if (!isSitting_) {
		status_.animNo = animType_[AnimType::idleToSitup];
		isSitting_ = true;
		status_.isAnimLoop = false;
		PModel_->ChangeAnimation(status_.animNo, status_.isAnimLoop, false, 20);
	}

	//座る過程のアニメーションが終わったら三角座りにする
	if (status_.animNo == animType_[AnimType::idleToSitup] && PModel_->IsAnimEnd()) {
		status_.animNo = animType_[AnimType::sit];
		PModel_->ChangeAnimation(status_.animNo, status_.isAnimLoop, false, 20);
		updateFunc_ = &Player::SitUpdate;
	}

}

//立ち上がる処理
void Player::StandUpdate(const InputState& input)
{
	if (PModel_->IsAnimEnd()) {
		updateFunc_ = &Player::IdleUpdate;
		isClim_ = false;
	}
}

void Player::SetCarryInfo(bool isCarry, shared_ptr<Model>model) {
	status_.isCarry = isCarry;
	temporaryCustodyPointer_ = model;
}

//セーブデータ
void Player::SetSaveData(VECTOR pos,int num, bool isContinue)
{
	checkPoint_ = pos;
	deathCount_ = num;
	isContinue_ = isContinue;
}

void Player::CarryObjectUpdate(const InputState& input) {

	if (input.IsPressed(InputType::carry) && status_.isCarry) {
		if (status_.animNo != animType_[AnimType::carryWalking]) {
			status_.animNo = animType_[AnimType::carryWalking];
		}
		temporaryCustodyPointer_->SetRot(VGet(status_.rot.x, status_.rot.y * DX_PI_F / 180.0f, status_.rot.z));
		temporaryCustodyPointer_->SetPos(FramPosition("mixamorig:LeftHand", "mixamorig:RightHand"));
	}
	else {
		bool isCarryWalking = status_.animNo == animType_[AnimType::carryWalking];
		bool isCarry = status_.animNo == animType_[AnimType::carry];
		if ((isCarryWalking || isCarry) && status_.isCarry) {
			status_.isCarry = false;
			temporaryCustodyPointer_->SetPos(FramPosition("mixamorig:LeftToeBase", "mixamorig:RightToeBase"));
			temporaryCustodyPointer_.reset();
		}
	}

}

//プレイヤーの速度設定
float Player::PlayerSpeed(bool pressedShift)
{
	if (pressedShift) return playerInfo_.runningSpeed;
	
	return playerInfo_.walkSpeed;
}


VECTOR Player::FramPosition(const char* const LeftFramename, const char* const RightFramename)
{

	VECTOR framePosition;

	//指定フレームの座標を取得し、二つの座標を足し、2で割り中心を取得する
	framePosition = PModel_->GetAnimFrameLocalPosition(status_.animNo, LeftFramename);
	framePosition = VAdd(framePosition, PModel_->GetAnimFrameLocalPosition(status_.animNo, RightFramename));
	framePosition.x = framePosition.x / 2;
	framePosition.y = framePosition.y / 2;
	framePosition.z = framePosition.z / 2;

	return framePosition;
}