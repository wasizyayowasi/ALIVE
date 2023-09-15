#include "Player.h"
#include "../UTIL/InputState.h"
#include "../UTIL/Model.h"
#include "../UTIL/CheckCollisionModel.h"
#include "../UTIL/LoadExternalFile.h"
#include<algorithm>
#include<string>

namespace {
	//重力
	constexpr float gravity = -0.4f;

	//ファイルパス
	const char* const player_Filename = "DATA/player/player11.mv1";
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

Player::Player():updateFunc_(&Player::idleUpdate)
{
	//プレイヤー情報の初期化
	{
		playerInfo.jumpPower = LoadExternalFile::getInstance().getPlayerInfo().jumpPower;
		playerInfo.runningJumpPower = LoadExternalFile::getInstance().getPlayerInfo().runningJumpPower;
		playerInfo.walkSpeed = LoadExternalFile::getInstance().getPlayerInfo().walkSpeed;
		playerInfo.runningSpeed = LoadExternalFile::getInstance().getPlayerInfo().runningSpeed;
		playerInfo.rotSpeed = LoadExternalFile::getInstance().getPlayerInfo().rotSpeed;
		for (int i = 0; i < static_cast<int>(AnimType::num); i++) {
			playerInfo.anim_[i] = LoadExternalFile::getInstance().getPlayerInfo().animNo_[i];
		}
	}

	for (int i = 0; i < static_cast<int>(AnimType::num); i++)
	{
		animType_[static_cast<AnimType>(i)] = playerInfo.anim_[i];
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

	//ジャンプ情報の初期
	jump_.isJump = false;
	jump_.jumpVec = 0.0f;

}

/// <summary>
/// デストラクタ
/// </summary>
Player::~Player()
{
}

/// <summary>
/// プレイヤーの更新を行う
/// </summary>
/// <param name="input">外部装置の入力情報を参照する</param>
/// <param name="models">衝突判定を行うモデルのvector型の配列</param>
void Player::update(const InputState& input, std::vector<std::shared_ptr<Model>> models)
{
	//移動ベクトルのリセット
	moveVec_ = { 0.0f,0.0f,0.0f };
	//移動スピードのリセット
	movingSpeed_ = 0.0f;

	//プレイヤーのアニメーション更新
	PModel_->update();
	
	(this->*updateFunc_)(input);

	//死体のvector配列を引数のmodels配列に追加する
	for (auto& deadPerson : deadPlayer_) {
		models.push_back(deadPerson);
	}

	//プレイヤーとその他オブジェクトとの衝突判定
	checkCollisionModel_->checkCollision(*this,moveVec_, models, player_hegiht, jump_.isJump, jump_.jumpVec);
}

/// <summary>
/// プレイヤー関連の描画
/// </summary>
void Player::draw()
{
	PModel_->draw();
	for (auto& deadPlayer : deadPlayer_) {
		if (deadPlayer->getEnable()) {
			deadPlayer->draw();
		}
	}
	
	DrawFormatString(0,32, 0x448844, "%d", animNo_);

	for (const auto person : deadPlayer_) {
		if (person->getEnable()) {
			DrawSphere3D(person->getPos(), 16, 32, 0xff0000, 0xff0000, true);
		}
	}
}

VECTOR Player::getRot()
{
	return {rot_.x, rot_.y * DX_PI_F / 180.0f, rot_.z};
}

/// <summary>
/// 外部からのポジションを受け取る
/// </summary>
/// <param name="pos">ポジション情報</param>
void Player::setPos(VECTOR pos)
{
	pos_ = pos;
	PModel_->setPos(pos_);
}

/// <summary>
/// 外部からのジャンプ情報を受け取る
/// </summary>
/// <param name="isJump">ジャンプフラグ</param>
/// <param name="jumpVec">ジャンプベクトル</param>
void Player::setJumpInfo(bool isJump, float jumpVec)
{
	jump_.isJump = isJump;
	jump_.jumpVec = jumpVec;
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
		updateFunc_ = &Player::sitUpdate;
		return;
	}

	//メンバ関数ポインタをrunningJumpUpdate、
	//jumpUpdateのどちらかに変更する
	if (input.isPressed(InputType::space)) {
		if (input.isPressed(InputType::shift)) {
			updateFunc_ = &Player::runningJumpUpdate;
			return;
		}
		else {
			updateFunc_ = &Player::jumpUpdate;
			return;
		}
	}

	//死亡演出中でなければ
	if (!isDead_) {
		changeAnimIdle();
		movingUpdate(input);
		death(input);
	}
	else {
		if (PModel_->isAnimEnd()) {
			//チェックポイントにプレイヤーを帰す
			pos_ = checkPoint_;
			//死亡演出が終了したことにする
			isDead_ = false;

			//死体を生成する関数
			deadPersonGenerater();

			//死体に指定アニメーションの最終フレームを設定する
			deadPlayer_.back()->setAnimEndFrame(animNo_);
			
			//アニメーション番号によって衝突判定用のフレームを変更する
			switch (static_cast<AnimType>(animNo_)) {
			case AnimType::death:
				deadPlayer_.back()->setCollFrame(coll_frame_death);
				break;
			case AnimType::sit:
				deadPlayer_.back()->setCollFrame(coll_frame_Sit);
				break;
			}
		}
	}

	//TODO：↓なくしたい
	{
		if (jump_.isJump && jump_.jumpVec == 0.0f) {
			pos_.y += gravity * 20;
		}

		if (pos_.y <= -400.0f) {
			pos_ = checkPoint_;
		}
		if (input.isTriggered(InputType::next)) {
			pos_ = checkPoint_;
		}
	}
	
}


//HACK:↓汚い、気に食わない
/// <summary>
/// プレイヤーを移動させるための関数
/// </summary>
/// <param name="input">外部装置の入力情報を参照する</param>
void Player::movingUpdate(const InputState& input)
{

	isMoving = false;
	
	//改善しよう
	{
		//HACK：汚い、リファクタリング必須
		if (input.isPressed(InputType::up)) {
			movingSpeed_ = playerInfo.walkSpeed;
			moveVec_.z += movingSpeed_;
			isMoving = true;
			targetAngle_ = 180.0f;
		}
		if (input.isPressed(InputType::down)) {
			movingSpeed_ = playerInfo.walkSpeed;
			moveVec_.z -= movingSpeed_;
			isMoving = true;
			targetAngle_ = 0.0f;
		}
		if (input.isPressed(InputType::left)) {
			movingSpeed_ = playerInfo.walkSpeed;
			moveVec_.x -= movingSpeed_;
			isMoving = true;
			targetAngle_ = 90.0f;
		}
		if (input.isPressed(InputType::right)) {
			movingSpeed_ = playerInfo.walkSpeed;
			moveVec_.x += movingSpeed_;
			isMoving = true;
			targetAngle_ = 270.0f;
		}
		if (input.isPressed(InputType::up) && input.isPressed(InputType::right)) {
			targetAngle_ = 225.0f;
		}
		if (input.isPressed(InputType::up) && input.isPressed(InputType::left)) {
			targetAngle_ = 135.0f;
		}
		if (input.isPressed(InputType::down) && input.isPressed(InputType::left)) {
			targetAngle_ = 45.0f;
		}
		if (input.isPressed(InputType::down) && input.isPressed(InputType::right)) {
			targetAngle_ = 315.0f;
		}
		if ((input.isPressed(InputType::up) || input.isPressed(InputType::down) || input.isPressed(InputType::left) || input.isPressed(InputType::right)) && input.isPressed(InputType::shift)) {
			movingSpeed_ = playerInfo.runningSpeed;
		}

		//HACK：もっといいアニメーション番号変更があるはず
		if (animNo_ != animType_[AnimType::runningJump] && animNo_ != animType_[AnimType::jump]) {
			if (movingSpeed_ != 0.0f) {
				if (movingSpeed_ > playerInfo.walkSpeed) {
					animNo_ = animType_[AnimType::run];
					isAnimLoop_ = true;
				}
				else if (movingSpeed_ <= playerInfo.walkSpeed) {
					animNo_ = animType_[AnimType::walk];
					isAnimLoop_ = true;
				}
			}
		}
		
		//移動ベクトルを用意する
		moveVec_ = VScale(VNorm(moveVec_),movingSpeed_);

		//回転処理
		rotationUpdate();

		//アニメーションの変更
		PModel_->changeAnimation(animNo_, isAnimLoop_, false, 20);

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
		if (!jump_.isJump && animNo_ != animType_[AnimType::jump]) {
			animNo_ = animType_[AnimType::jump];
			jump_.jumpVec += playerInfo.jumpPower;
			jump_.isJump = true;
			isAnimLoop_ = false;
			PModel_->changeAnimation(animNo_, isAnimLoop_, false, 20);
		}

		//空中にいるとき
		//重力をベクトルに足してポジションに足す
		if (jump_.isJump) {
			jump_.jumpVec += gravity;
			pos_.y += jump_.jumpVec;
		}

		//ジャンプベクトルが0でジャンプ中ではなかったら
		//idle状態のアップデートに変更する、アニメーションも変更する
		if (jump_.jumpVec == 0.0f && !jump_.isJump) {
			updateFunc_ = &Player::idleUpdate;
			animNo_ = animType_[AnimType::idle];
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
	if (!jump_.isJump&& animNo_ != animType_[AnimType::runningJump]) {
		animNo_ = animType_[AnimType::runningJump];
		jump_.jumpVec += playerInfo.runningJumpPower;
		isAnimLoop_ = false;
		jump_.isJump = true;
		PModel_->changeAnimation(animNo_, isAnimLoop_, false, 20);
	}

	//HACK：変数が仮のまま　+　どうするか悩んでいる
	//アニメーションの総時間によって、重力を変更する
	temp = PModel_->getAnimTotalTime() + 2;
	tempGravity = -(playerInfo.runningJumpPower * 2 / temp);

	//空中にいるとき
	//重力をベクトルに足してポジションに足す
	if (jump_.isJump) {
		jump_.jumpVec += tempGravity;
		pos_.y += jump_.jumpVec;
	}

	//ジャンプベクトルが0でジャンプ中ではなかったら
	//idle状態のアップデートに変更する、アニメーションも変更する
	if (jump_.jumpVec == 0.0f && !jump_.isJump) {
		updateFunc_ = &Player::idleUpdate;
		animNo_ = animType_[AnimType::run];
		isAnimLoop_ = true;
		return;
	}
}

/// <summary>
/// プレイヤーの死体に与える情報を作る関数
/// </summary>
/// <param name="input">外部装置の入力情報を参照する</param>
void Player::death(const InputState& input)
{
	//死亡
	{
		if (input.isTriggered(InputType::death)) {
			deathPos = pos_;				//死んだ場所を残す
			
			isAnimLoop_ = false;			
			isDead_ = true;

			//座るアニメーション以外だったら死ぬアニメーションに変える
			if (animNo_ != animType_[AnimType::sit]) {
				animNo_ = animType_[AnimType::death];
				PModel_->changeAnimation(animNo_, isAnimLoop_, false, 20);
			}
		}
	}
}

/// <summary>
/// アニメーションをidleに戻す関数
/// </summary>
void Player::changeAnimIdle()
{
	//待機アニメーションに戻す
	if (!isMoving) {
		animNo_ = animType_[AnimType::idle];
		isAnimLoop_ = true;
		PModel_->changeAnimation(animNo_, isAnimLoop_, false, 20);
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
		rot_.y -= playerInfo.rotSpeed;
		tempAngle_ -= playerInfo.rotSpeed;
	}
	else if(differenceAngle_ > 0.0f){
		rot_.y += playerInfo.rotSpeed;
		tempAngle_ += playerInfo.rotSpeed;
	}

	//360度、一周したら0度に戻すようにしている
	if (tempAngle_ == 360.0f || tempAngle_ == -360.0f) {
		tempAngle_ = 0.0f;
	}
	if (rot_.y == 360.0f || rot_.y == -360.0f) {
		rot_.y = 0.0f;
	}

	//結果をモデルの回転情報として送る
	PModel_->setRot({ rot_.x,rot_.y * DX_PI_F / 180.0f,rot_.z });
}

/// <summary>
/// プレイヤーの死体をvector配列で管理する関数
/// </summary>
void Player::deadPersonGenerater()
{
	//死体の生成
	deadPlayer_.push_back(make_shared<Model>(PModel_->getModelHandle()));
	//死体のポジション設定
	deadPlayer_.back()->setPos(deathPos);
	//死体のサイズ設定
	deadPlayer_.back()->setScale(player_scale);
	//死体の回転設定
	deadPlayer_.back()->setRot({ rot_.x,rot_.y * DX_PI_F / 180.0f,rot_.z });
	
	//死体を数える
	int deathNum = 0;
	for (const auto person : deadPlayer_) {
		deathNum++;
		if (deathNum > 10) {
			deadPlayer_.erase(deadPlayer_.begin());		//最大数を超えたら一番古い死体を消す
		}
	}
}

/// <summary>
/// プレイヤーに座るアニメーションをさせる関数
/// </summary>
/// <param name="input">外部装置の入力情報を参照する</param>
void Player::sitUpdate(const InputState& input)
{
	//座る過程のアニメーションが終わったら三角座りにする
	if (animNo_ == animType_[AnimType::idleToSitup] && PModel_->isAnimEnd()) {
		animNo_ = animType_[AnimType::sit];
		PModel_->changeAnimation(animNo_, isAnimLoop_, false, 20);
	}

	//立つ家庭のアニメーションが終わったらidleupdateに変更する
	if (animNo_ == animType_[AnimType::situpToIdle] && PModel_->isAnimEnd()) {
		updateFunc_ = &Player::idleUpdate;
		isSitting_ = false;
		return;
	}

	//アニメーションを座る過程のアニメーションに変更
	//座っているフラグを立て、アニメーションループ変数を折る
	if (!isSitting_) {
		animNo_ = animType_[AnimType::idleToSitup];
		isSitting_ = true;
		isAnimLoop_ = false;
		PModel_->changeAnimation(animNo_, isAnimLoop_, false, 20);
	}

	//死ぬコマンド
	if (input.isTriggered(InputType::death)) {
		death(input);
		isSitting_ = false;
		updateFunc_ = &Player::idleUpdate;
		return;
	}

	//立ち上がるためのコマンド
	if(input.isTriggered(InputType::ctrl)){
		animNo_ = animType_[AnimType::situpToIdle];
		isAnimLoop_ = false;
		PModel_->changeAnimation(animNo_, isAnimLoop_, false, 20);
	}
}
