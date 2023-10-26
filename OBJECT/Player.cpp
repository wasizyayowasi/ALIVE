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
	const char* const player_Filename = "data/player/temp.mv1";
	//モデルフレーム名
	const char* const coll_frame_death = "CollisionDeath";
	const char* const coll_frame_Sit = "CollisionSit";
	const char* const coll_frame_Stand = "CollisionStand";

	//フレームの名前
	const char* const frame_name = "foot.L";

	//プレイヤーサイズ
	const VECTOR player_scale = { 1.0f,1.0f ,1.0f };

	//初期プレイヤーの回転角度
	const VECTOR start_player_rot = { 0.0f,0.0f ,0.0f };

	//プレイヤーの高さ
	constexpr float player_hegiht = 150.0f;

}

using namespace std;

Player::Player(const char* const filename):updateFunc_(&Player::IdleUpdate),carryUpdateFunc_(&Player::CarryObjectUpdate)
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


void Player::Init(LoadObjectInfo info)
{

	auto loadExternalFile = LoadExternalFile::GetInstance();
	//プレイヤー情報の初期化
	{
		playerInfo_.jumpPower = loadExternalFile.GetPlayerInfo().jumpPower;
		playerInfo_.runningJumpPower = loadExternalFile.GetPlayerInfo().runningJumpPower;
		playerInfo_.walkSpeed = loadExternalFile.GetPlayerInfo().walkSpeed;
		playerInfo_.runningSpeed = loadExternalFile.GetPlayerInfo().runningSpeed;
		playerInfo_.rotSpeed = loadExternalFile.GetPlayerInfo().rotSpeed;
		for (int i = 0; i < static_cast<int>(AnimType::max); i++) {
			playerInfo_.animNo_[i] = loadExternalFile.GetPlayerInfo().animNo_[i];
		}
	}

	for (int i = 0; i < static_cast<int>(AnimType::max); i++)
	{
		animType_[static_cast<AnimType>(i)] = playerInfo_.animNo_[i];
	}

	//プレイヤーモデルの生成
	PModel_ = make_shared<Model>(player_Filename);
	//アニメーションの設定
	PModel_->SetAnimation(animType_[AnimType::idle], true, false);
	//プレイヤーの大きさの調整
	PModel_->SetScale(info.scale);
	//ポジションの設定
	PModel_->SetPos(info.pos);
	//回転率の設定
	PModel_->SetPos(info.rot);
	//マップやブロックなどの当たり判定の生成
	checkCollisionModel_ = make_shared<CheckCollisionModel>();
	//コリジョンフレームの設定
	PModel_->SetCollFrame("Character");

	scale_ = info.scale;

}



void Player::Update(const InputState& input)
{
	//移動ベクトルのリセット
	status_.moveVec = { 0.0f,0.0f,0.0f };

	//プレイヤーのアニメーション更新
	PModel_->Update();
	
	(this->*updateFunc_)(input);

	//プレイヤーとその他オブジェクトとの衝突判定
	checkCollisionModel_->CheckCollision(*this);
}


void Player::Draw()
{
	PModel_->Draw();
//	VECTOR aiu = FramPosition2("hand.R_end");
//	DrawFormatString(0, 48, 0x448844, "%.2f , %.2f , %.2f", aiu.x, aiu.y, aiu.z);
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
	

	if (input.IsTriggered(InputType::carry)) {
		(this->*carryUpdateFunc_)();
	}

	if (status_.isTransit) {
		deadPersonModelPointer_->SetAnimEndFrame(animType_[AnimType::dead]);
		deadPersonModelPointer_->SetRot(DegreesToRadians(status_.rot));
		deadPersonModelPointer_->SetPos(FramPosition2("hand.R_end"));
	}
	else {
		isCanBeCarried_ = false;
	}

	ChangeAnimIdle();
	MovingUpdate(input);

	//持ち運び中だったら
	//以降の処理を行わない
	if (status_.isTransit) {
		return;
	}

	//メンバ関数ポインタをrunningJumpUpdate、
	//jumpUpdateのどちらかに変更する
	if (input.IsTriggered(InputType::space)) {

		if (isClim_) {
			//アニメーションの変更
			//ChangeAnimNo(AnimType::clim, false, 20);
			//updateFunc_ = &Player::ClimUpdate;
			return;
		}
		//ランニングジャンプの削除予定(未定)
		/*else if (input.IsPressed(InputType::shift)) {
			PlayerJump(playerInfo_.runningJumpPower);
			ChangeAnimNo(AnimType::runningJump, false, 20);
			updateFunc_ = &Player::RunningJumpUpdate;
			return;
		}*/
		else {
			PlayerJump(playerInfo_.jumpPower);
			ChangeAnimNo(AnimType::jump, false, 20);
			updateFunc_ = &Player::JumpUpdate;
			return;
		}
	}

	//メンバ関数ポインタをsitUpdateに変更する
	if (input.IsTriggered(InputType::ctrl)) {
		updateFunc_ = &Player::IdleToSitup;
		return;
	}

	//メンバ関数ポインタをDeathUpdateに変更する
	if (input.IsTriggered(InputType::death)) {
		deathCount_++;
		updateFunc_ = &Player::DeathUpdate;
		return;
	}

}

/// <summary>
/// アニメーションをidleに戻す関数
/// </summary>
void Player::ChangeAnimIdle()
{
	//待機アニメーションに戻す
	if (!isMoving_) {

		if (status_.isTransit) {
			ChangeAnimNo(AnimType::carryIdle, true, 20);
		}
		else {
			ChangeAnimNo(AnimType::idle, true, 20);
		}
	}
}

//HACK:↓汚い、気に食わない
/// <summary>
/// プレイヤーを移動させるための関数
/// </summary>
/// <param name="input">外部装置の入力情報を参照する</param>
void Player::MovingUpdate(const InputState& input)
{
	
	float movingSpeed = Move(input);

	//HACK：もっといいアニメーション番号変更があるはず
	if (movingSpeed != 0.0f) {
		if (movingSpeed > playerInfo_.walkSpeed) {
			//アニメーションの変更
			if (status_.isTransit) {
				ChangeAnimNo(AnimType::carryRunning, true, 20);
			}
			else {
				ChangeAnimNo(AnimType::run, true, 20);
			}
		}
		else if (movingSpeed <= playerInfo_.walkSpeed) {
			//アニメーションの変更
			if (status_.isTransit) {
				ChangeAnimNo(AnimType::carryWalking, true, 20);
			}
			else {
				ChangeAnimNo(AnimType::walk, true, 20);
			}
		}
	}

	if (VSize(status_.moveVec) == 0.0f) {
		isMoving_ = false;
		return;
	}

	//移動ベクトルを用意する
	status_.moveVec = VScale(VNorm(status_.moveVec), movingSpeed);

}

float Player::Move(const InputState& input) {
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

	//回転処理
	RotationUpdate();

	return movingSpeed;

}

//完成品だから今後いじらなくていいと思う
/// <summary>
/// プレイヤーの回転処理を行う関数
/// </summary>
void Player::RotationUpdate()
{
	//目標の角度から現在の角度を引いて差を出している
	differenceAngle_ = targetAngle_ - angle_;

	//常にプレイヤーモデルを大周りさせたくないので
	//181度又は-181度以上の場合、逆回りにしてあげる
	if (differenceAngle_ >= 180.0f) {
		differenceAngle_ = targetAngle_ - angle_ - 360.0f;
	}
	else if (differenceAngle_ <= -180.0f) {
		differenceAngle_ = targetAngle_ - angle_ + 360.0f;
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
	if (angle_ == 360.0f || angle_ == -360.0f) {
		angle_ = 0.0f;
	}
	if (status_.rot.y == 360.0f || status_.rot.y == -360.0f) {
		status_.rot.y = 0.0f;
	}

	//結果をモデルの回転情報として送る
	PModel_->SetRot(DegreesToRadians(status_.rot));
}

//オブジェクトを登る
void Player::ClimUpdate(const InputState& input)
{
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
	Move(input);

	//ジャンプ処理
	{
		//ジャンプベクトルが0でジャンプ中ではなかったら
		//idle状態のアップデートに変更する、アニメーションも変更する
		if (!status_.jump.isJump) {
			updateFunc_ = &Player::IdleUpdate;
			return;
		}

		//空中にいるとき
		//重力をベクトルに足してポジションに足す
		if (status_.jump.isJump) {
			status_.jump.jumpVec += gravity;
			status_.pos.y += status_.jump.jumpVec;
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
	Move(input);

	//HACK：変数が仮のまま　+　どうするか悩んでいる
	//アニメーションの総時間によって、重力を変更する
	totalAnimFrame_ = PModel_->GetAnimTotalTime() + 2;
	runJumpGravity = -(playerInfo_.runningJumpPower * 2 / totalAnimFrame_);

	//空中にいるとき
	//重力をベクトルに足してポジションに足す
	if (status_.jump.isJump) {
		status_.jump.jumpVec += runJumpGravity;
		status_.pos.y += status_.jump.jumpVec;
	}

	//ジャンプベクトルが0でジャンプ中ではなかったら
	//idle状態のアップデートに変更する、アニメーションも変更する
	if (status_.jump.jumpVec == 0.0f && !status_.jump.isJump) {
		updateFunc_ = &Player::IdleUpdate;
		return;
	}
}

/// <summary>
/// プレイヤーの死体に与える情報を作る関数
/// </summary>
/// <param name="input">外部装置の入力情報を参照する</param>
void Player::DeathUpdate(const InputState& input)
{
	deathPos_ = status_.pos;				//死んだ場所を残す

	//座るアニメーション以外だったら死ぬアニメーションに変える
	if (status_.animNo != animType_[AnimType::idleToSitup]) {
		//アニメーションの変更
		ChangeAnimNo(AnimType::death, false, 20);
	}

	if (PModel_->IsAnimEnd()) {
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

	LoadObjectInfo info;
	info.rot = DegreesToRadians(status_.rot);
	info.pos = deathPos_;
	info.scale = scale_;

	objManager.DeadPersonGenerator(PModel_->GetModelHandle(),info, status_.animNo);
}

/// <summary>
/// プレイヤーに座るアニメーションをさせる関数
/// </summary>
/// <param name="input">外部装置の入力情報を参照する</param>
void Player::SitUpdate(const InputState& input)
{
	//立ち上がるためのコマンド
	if (input.IsTriggered(InputType::ctrl)) {
		//アニメーションの変更
		ChangeAnimNo(AnimType::situpToIdle, false, 20);
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
		isSitting_ = true;
		ChangeAnimNo(AnimType::idleToSitup, false, 20);
	}

	//座る過程のアニメーションが終わったら三角座りにする
	if (status_.animNo == animType_[AnimType::idleToSitup] && PModel_->IsAnimEnd()) {
		PModel_->SetAnimEndFrame(status_.animNo);
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
	isCanBeCarried_ = isCarry;
	deadPersonModelPointer_ = model;
}

void Player::CarryObjectUpdate()
{
	
	if (!isCanBeCarried_) return;

	status_.isTransit = true;

	carryUpdateFunc_ = &Player::DropOffObjectUpdate;

}

//運んでいたオブジェクトを下ろす処理
void Player::DropOffObjectUpdate()
{
	bool isCarryWalking = status_.animNo == animType_[AnimType::carryWalking];
	bool isCarry = status_.animNo == animType_[AnimType::carryIdle];
	if ((isCarryWalking || isCarry) && isCanBeCarried_) {
		isCanBeCarried_ = false;
		deadPersonModelPointer_->SetPos(FramPosition("foot.L", "foot.R"));
		deadPersonModelPointer_.reset();
	}

	status_.isTransit = false;
	isCanBeCarried_ = false;

	carryUpdateFunc_ = &Player::CarryObjectUpdate;

}

//度数法から弧度法に変換した角度を返す
VECTOR Player::DegreesToRadians(VECTOR rot)
{
	VECTOR storageRot;

	storageRot.x = rot.x * DX_PI_F / 180.0f;
	storageRot.y = rot.y * DX_PI_F / 180.0f;
	storageRot.z = rot.z * DX_PI_F / 180.0f;

	return storageRot;
}

void Player::ChangeAnimNo(AnimType type, bool isAnimLoop, int changeTime)
{
	status_.animNo = animType_[type];
	status_.isAnimLoop = isAnimLoop;
	PModel_->ChangeAnimation(status_.animNo, status_.isAnimLoop, false, changeTime);
}

//プレイヤーの速度設定
float Player::PlayerSpeed(bool pressedShift)
{
	if (pressedShift) return playerInfo_.runningSpeed;
	
	return playerInfo_.walkSpeed;
}

//ジャンプの設定
void Player::PlayerJump(float jumpPower) {
	status_.jump.jumpVec += jumpPower;
	status_.pos.y += status_.jump.jumpVec;
	status_.jump.isJump = true;
}

//二つのフレーム座標の中心を取得する
VECTOR Player::FramPosition(const char* const LeftFramename, const char* const RightFramename)
{

	VECTOR framePosition;

	//指定フレームの座標を取得する。
	framePosition = PModel_->GetAnimFrameLocalPosition(LeftFramename);
	framePosition = VAdd(framePosition, PModel_->GetAnimFrameLocalPosition(RightFramename));
	//二つの座標を足し、2で割り中心を取得する
	framePosition.x = framePosition.x / 2;
	framePosition.y = framePosition.y / 2;
	framePosition.z = framePosition.z / 2;

	return framePosition;
}

VECTOR Player::FramPosition2(const char* const framename)
{

	VECTOR framePosition;

	//指定フレームの座標を取得する。
	framePosition = PModel_->GetAnimFrameLocalPosition(framename);

	return framePosition;
}
