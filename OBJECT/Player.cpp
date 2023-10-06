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
	const char* const player_Filename = "DATA/player/player14.mv1";
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

Player::Player(const char* const filename):updateFunc_(&Player::idleUpdate)
{

	//ジャンプ情報の初期
	status_.jump.isJump = false;
	status_.jump.jumpVec = 0.0f;
}

Player::Player(int handle) :updateFunc_(&Player::idleUpdate)
{
}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
}


void Player::init()
{

	auto loadExternalFile = LoadExternalFile::getInstance(isContinue_);
	//プレイヤー情報の初期化
	{
		playerInfo_.jumpPower = loadExternalFile.getPlayerInfo().jumpPower;
		playerInfo_.runningJumpPower = loadExternalFile.getPlayerInfo().runningJumpPower;
		playerInfo_.walkSpeed = loadExternalFile.getPlayerInfo().walkSpeed;
		playerInfo_.runningSpeed = loadExternalFile.getPlayerInfo().runningSpeed;
		playerInfo_.rotSpeed = loadExternalFile.getPlayerInfo().rotSpeed;
		for (int i = 0; i < static_cast<int>(AnimType::max); i++) {
			playerInfo_.anim_[i] = loadExternalFile.getPlayerInfo().animNo_[i];
		}
	}

	for (int i = 0; i < static_cast<int>(AnimType::max); i++)
	{
		animType_[static_cast<AnimType>(i)] = playerInfo_.anim_[i];
	}

	//プレイヤーモデルの生成
	PModel_ = make_shared<Model>(player_Filename);
	//アニメーションの設定
	PModel_->setAnimation(animType_[AnimType::idle], true, false);
	//プレイヤーの大きさの調整
	PModel_->setScale(player_scale);
	//マップやブロックなどの当たり判定の生成
	checkCollisionModel_ = make_shared<CheckCollisionModel>();
	//コリジョンフレームの設定
	PModel_->setCollFrame("Character");
}



void Player::update(const InputState& input)
{
	//移動ベクトルのリセット
	status_.moveVec = { 0.0f,0.0f,0.0f };

	//プレイヤーのアニメーション更新
	PModel_->update();
	
	(this->*updateFunc_)(input);

	//プレイヤーとその他オブジェクトとの衝突判定
	checkCollisionModel_->checkCollision(*this, status_.moveVec,player_hegiht, status_.jump.isJump, status_.jump.jumpVec);
}


void Player::draw()
{
	PModel_->draw();

	DrawFormatString(0, 48, 0x448844, "%.2f", tempAngle_);
	DrawFormatString(0, 64, 0x448844, "%d", deathCount_);

}

VECTOR Player::getRot()
{
	return { status_.rot.x, status_.rot.y * DX_PI_F / 180.0f, status_.rot.z};
}


void Player::setPos(VECTOR pos)
{
	status_.pos = pos;
	PModel_->setPos(status_.pos);
}


void Player::setJumpInfo(bool isJump, float jumpVec)
{
	status_.jump.isJump = isJump;
	status_.jump.jumpVec = jumpVec;
}


//HACK:↓汚い、気に食わない
/// <summary>
/// アニメーションがidle状態の時に行われる
/// </summary>
/// <param name="input">外部装置の入力情報を参照する</param>
void Player::idleUpdate(const InputState& input)
{

	//メンバ関数ポインタをsitUpdateに変更する
	if (input.isTriggered(InputType::ctrl)) {
		updateFunc_ = &Player::idleToSitup;
		return;
	}

	//メンバ関数ポインタをrunningJumpUpdate、
	//jumpUpdateのどちらかに変更する
	if (input.isTriggered(InputType::space)) {
		if (isClim_) {
			updateFunc_ = &Player::climUpdate;
			return;
		}
		else if (input.isPressed(InputType::shift)) {
			updateFunc_ = &Player::runningJumpUpdate;
			return;
		}
		else {
			updateFunc_ = &Player::jumpUpdate;
			return;
		}
	}

	if (input.isTriggered(InputType::death)) {
		updateFunc_ = &Player::deathUpdate;
		return;
	}

	changeAnimIdle();
	movingUpdate(input);

	//TODO：↓なくしたい
	{
		if (status_.jump.isJump && status_.jump.jumpVec == 0.0f) {
			status_.pos.y += gravity * 20;
		}

		if (status_.pos.y <= -400.0f) {
			status_.pos = checkPoint_;
		}
		if (input.isTriggered(InputType::space)) {
			status_.pos = checkPoint_;
		}
	}
	
}

/// <summary>
/// アニメーションをidleに戻す関数
/// </summary>
void Player::changeAnimIdle()
{
	//待機アニメーションに戻す
	if (!isMoving_) {
		status_.animNo = animType_[AnimType::idle];
		status_.isAnimLoop = true;
		PModel_->changeAnimation(status_.animNo, status_.isAnimLoop, false, 20);
	}
}

//HACK:↓汚い、気に食わない
/// <summary>
/// プレイヤーを移動させるための関数
/// </summary>
/// <param name="input">外部装置の入力情報を参照する</param>
void Player::movingUpdate(const InputState& input)
{
	//キーの押下をブール型に格納
	bool pressedUp = input.isPressed(InputType::up);
	bool pressedDown = input.isPressed(InputType::down);
	bool pressedLeft = input.isPressed(InputType::left);
	bool pressedRight = input.isPressed(InputType::right);
	bool pressedShift = input.isPressed(InputType::shift);

	isMoving_ = false;
	float movingSpeed = 0.0f;

	if (pressedUp || pressedDown || pressedLeft || pressedRight) {
		movingSpeed = playerSpeed(pressedShift);
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
		if (status_.animNo != animType_[AnimType::runningJump] && status_.animNo != animType_[AnimType::jump]) {
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
		rotationUpdate();

		if (VSize(status_.moveVec) == 0.0f) {
			isMoving_ = false;
			return;
		}
		//移動ベクトルを用意する
		status_.moveVec = VScale(VNorm(status_.moveVec), movingSpeed);

		//アニメーションの変更
		PModel_->changeAnimation(status_.animNo, status_.isAnimLoop, false, 20);


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
void Player::rotationUpdate()
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
	PModel_->setRot({ status_.rot.x,status_.rot.y * DX_PI_F / 180.0f,status_.rot.z });
}

//オブジェクトを登る
void Player::climUpdate(const InputState& input)
{
	status_.animNo = animType_[AnimType::clim];
	status_.isAnimLoop = false;

	PModel_->changeAnimation(status_.animNo, status_.isAnimLoop, false, 20);

	VECTOR localPosition;

	if (PModel_->isAnimEnd()) {
		localPosition = PModel_->getAnimFrameLocalPosition(status_.animNo, "mixamorig:LeftToeBase");
		localPosition = VAdd(localPosition, PModel_->getAnimFrameLocalPosition(status_.animNo, "mixamorig:RightToeBase"));
		localPosition.x = localPosition.x / 2;
		localPosition.y = localPosition.y / 2;
		localPosition.z = localPosition.z / 2;
		temp_ = localPosition;
		status_.pos = localPosition;
		PModel_->setPos(status_.pos);
		status_.animNo = animType_[AnimType::stand];
		status_.isAnimLoop = false;
		PModel_->setAnimation(status_.animNo, status_.isAnimLoop, true);
		updateFunc_ = &Player::standUpdate;
	}

}

//HACK:↓汚い、気に食わない
/// <summary>
/// 走りジャンプではないときのジャンプ
/// </summary>
/// <param name="input">外部装置の入力情報を参照する</param>
void Player::jumpUpdate(const InputState& input)
{
	//プレイヤー移動関数
	movingUpdate(input);

	//ジャンプ処理
	{
		//アニメーション変更と脚力をジャンプベクトルに足す
		if (!status_.jump.isJump && status_.animNo != animType_[AnimType::jump]) {
			status_.animNo = animType_[AnimType::jump];
			status_.jump.jumpVec += playerInfo_.jumpPower;
			status_.jump.isJump = true;
			status_.isAnimLoop = false;
			PModel_->changeAnimation(status_.animNo, status_.isAnimLoop, false, 20);
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
			updateFunc_ = &Player::idleUpdate;
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
void Player::runningJumpUpdate(const InputState& input)
{
	//プレイヤー移動関数
	movingUpdate(input);

	//アニメーション変更と脚力をジャンプベクトルに足す
	if (!status_.jump.isJump && status_.animNo != animType_[AnimType::runningJump]) {
		status_.animNo = animType_[AnimType::runningJump];
		status_.jump.jumpVec += playerInfo_.runningJumpPower;
		status_.isAnimLoop = false;
		status_.jump.isJump = true;
		PModel_->changeAnimation(status_.animNo, status_.isAnimLoop, false, 20);
	}

	//HACK：変数が仮のまま　+　どうするか悩んでいる
	//アニメーションの総時間によって、重力を変更する
	temp = PModel_->getAnimTotalTime() + 2;
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
		updateFunc_ = &Player::idleUpdate;
		status_.animNo = animType_[AnimType::run];
		status_.isAnimLoop = true;
		return;
	}
}

/// <summary>
/// プレイヤーの死体に与える情報を作る関数
/// </summary>
/// <param name="input">外部装置の入力情報を参照する</param>
void Player::deathUpdate(const InputState& input)
{
	deathPos = status_.pos;				//死んだ場所を残す

	status_.isAnimLoop = false;			//アニメーションのループをするか

	//座るアニメーション以外だったら死ぬアニメーションに変える
	if (status_.animNo != animType_[AnimType::sit]) {
		status_.animNo = animType_[AnimType::death];
		PModel_->changeAnimation(status_.animNo, status_.isAnimLoop, false, 20);
	}

	if (PModel_->isAnimEnd()) {
		deathCount_++;					//死亡回数をカウントする
		deathPersonPostProsessing();
	}

}

//死体の後処理
void Player::deathPersonPostProsessing()
{
	status_.pos = checkPoint_;				//チェックポイントにプレイヤーを帰す

	deadPersonGenerater();			//死体を生成する関数

	updateFunc_ = &Player::idleUpdate;
}

/// <summary>
/// プレイヤーの死体をvector配列で管理する関数
/// </summary>
void Player::deadPersonGenerater()
{
	auto& objManager = ObjectManager::getInstance();

	VECTOR rot = { status_.rot.x,status_.rot.y * DX_PI_F / 180.0f,status_.rot.z };

	objManager.deadPersonGenerator(ObjectType::deadPerson, PModel_->getModelHandle(), deathPos, rot, status_.animNo);
}

/// <summary>
/// プレイヤーに座るアニメーションをさせる関数
/// </summary>
/// <param name="input">外部装置の入力情報を参照する</param>
void Player::sitUpdate(const InputState& input)
{
	//立ち上がるためのコマンド
	if (input.isTriggered(InputType::ctrl)) {
		status_.animNo = animType_[AnimType::situpToIdle];
		status_.isAnimLoop = false;
		PModel_->changeAnimation(status_.animNo, status_.isAnimLoop, false, 20);
	}
	
	//立つ家庭のアニメーションが終わったらidleupdateに変更する
	if (status_.animNo == animType_[AnimType::situpToIdle] && PModel_->isAnimEnd()) {
		updateFunc_ = &Player::idleUpdate;
		isSitting_ = false;
		return;
	}

	if (status_.animNo == animType_[AnimType::situpToIdle]) return;

	//死ぬコマンド
	if (input.isTriggered(InputType::death)) {
		deathUpdate(input);
		isSitting_ = false;
		updateFunc_ = &Player::idleUpdate;
		return;
	}

}

void Player::idleToSitup(const InputState& input)
{
	//アニメーションを座る過程のアニメーションに変更
	//座っているフラグを立て、アニメーションループ変数を折る
	if (!isSitting_) {
		status_.animNo = animType_[AnimType::idleToSitup];
		isSitting_ = true;
		status_.isAnimLoop = false;
		PModel_->changeAnimation(status_.animNo, status_.isAnimLoop, false, 20);
	}

	//座る過程のアニメーションが終わったら三角座りにする
	if (status_.animNo == animType_[AnimType::idleToSitup] && PModel_->isAnimEnd()) {
		status_.animNo = animType_[AnimType::sit];
		PModel_->changeAnimation(status_.animNo, status_.isAnimLoop, false, 20);
		updateFunc_ = &Player::sitUpdate;
	}

}

//立ち上がる処理
void Player::standUpdate(const InputState& input)
{
	if (PModel_->isAnimEnd()) {
		updateFunc_ = &Player::idleUpdate;
		isClim_ = false;
	}
}

//セーブデータ
void Player::setSaveData(VECTOR pos,int num, bool isContinue)
{
	checkPoint_ = pos;
	deathCount_ = num;
	isContinue_ = isContinue;
}

//プレイヤーの速度設定
float Player::playerSpeed(bool pressedShift)
{
	if (pressedShift) return playerInfo_.runningSpeed;
	
	return playerInfo_.walkSpeed;
}