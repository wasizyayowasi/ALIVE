#include "Player.h"
#include "../UTIL/InputState.h"
#include "../UTIL/Model.h"
#include "../UTIL/CheckCollisionModel.h"
#include<algorithm>
#include<string>

namespace {
	//アニメーション
	constexpr int anim_idle_no = 0;			//待機状態
	constexpr int anim_run_no = 1;			//走る
	constexpr int anim_stairs_no = 2;		//階段を上る
	constexpr int anim_clim_no = 3;			//上る
	constexpr int anim_jump_no = 4;			//ジャンプ
	constexpr int anim_runningJump_no = 5;	//走りジャンプ
	constexpr int anim_death_no = 6;		//走りジャンプ
	constexpr int anim_walk_no = 7;			//歩く
	constexpr int anim_idleToSitup_no = 8;	//idle状態から座る
	constexpr int anim_situpToIdle_no = 9;	//座っている状態からidle
	constexpr int anim_sit_no = 10;			//座っている

	//ジャンプ
	constexpr float jump_power = 10.0f;
	constexpr float runningJump_power = 8.0f;
	constexpr float gravity = -0.4f;

	//回転スピード
	constexpr float rot_speed = 15.0f;

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

	//移動スピード
	constexpr float walk_speed = 3.0f;
	constexpr float run_speed = 7.0f;
}

using namespace std;

Player::Player():updateFunc_(&Player::idleUpdate)
{
	//プレイヤーモデルの生成
	PModel_ = make_shared<Model>(player_Filename);
	//アニメーションの設定
	PModel_->setAnimation(anim_idle_no, true, false);
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
	
	DrawFormatString(0, 32, 0x448844, "%d", animNo_);
	DrawFormatString(0, 48, 0x448844, "%d", jump_.isJump);
	DrawFormatString(0, 48, 0x448844, "%f.2", pos_.y);

	DrawSphere3D(pos_, 16, 32, 0x0000ff, 0x0000ff, true);

	for (const auto person : deadPlayer_) {
		if (person->getEnable()) {
			DrawSphere3D(person->getPos(), 16, 32, 0xff0000, 0xff0000, true);
		}
	}
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
			switch (animNo_) {
			case anim_death_no:
				deadPlayer_.back()->setCollFrame(coll_frame_death);
				break;
			case anim_sit_no:
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
			movingSpeed_ = walk_speed;
			moveVec_.z += movingSpeed_;
			isMoving = true;
			targetAngle_ = 180.0f;
		}
		if (input.isPressed(InputType::down)) {
			movingSpeed_ = walk_speed;
			moveVec_.z -= movingSpeed_;
			isMoving = true;
			targetAngle_ = 0.0f;
		}
		if (input.isPressed(InputType::left)) {
			movingSpeed_ = walk_speed;
			moveVec_.x -= movingSpeed_;
			isMoving = true;
			targetAngle_ = 90.0f;
		}
		if (input.isPressed(InputType::right)) {
			movingSpeed_ = walk_speed;
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
			movingSpeed_ = run_speed;
		}

		//HACK：もっといいアニメーション番号変更があるはず
		if (animNo_ != anim_runningJump_no && animNo_ != anim_jump_no) {
			if (movingSpeed_ != 0.0f) {
				if (movingSpeed_ > walk_speed) {
					animNo_ = anim_run_no;
					isAnimLoop_ = true;
				}
				else if (movingSpeed_ <= walk_speed) {
					animNo_ = anim_walk_no;
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
		if (!jump_.isJump && animNo_ != anim_jump_no) {
			animNo_ = anim_jump_no;
			jump_.jumpVec += jump_power;
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
			animNo_ = anim_idle_no;
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
	if (!jump_.isJump&& animNo_ != anim_runningJump_no) {
		animNo_ = anim_runningJump_no;
		jump_.jumpVec += runningJump_power;
		isAnimLoop_ = false;
		jump_.isJump = true;
		PModel_->changeAnimation(animNo_, isAnimLoop_, false, 20);
	}

	//HACK：変数が仮のまま　+　どうするか悩んでいる
	//アニメーションの総時間によって、重力を変更する
	temp = PModel_->getAnimTotalTime() + 2;
	tempGravity = -(runningJump_power * 2 / temp);

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
		animNo_ = anim_run_no;
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
			if (animNo_ != anim_sit_no) {
				animNo_ = anim_death_no;
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
		animNo_ = anim_idle_no;
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
		rot_.y -= rot_speed;
		tempAngle_ -= rot_speed;
	}
	else if(differenceAngle_ > 0.0f){
		rot_.y += rot_speed;
		tempAngle_ += rot_speed;
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
	if (animNo_ == anim_idleToSitup_no && PModel_->isAnimEnd()) {
		animNo_ = anim_sit_no;
		PModel_->changeAnimation(animNo_, isAnimLoop_, false, 20);
	}

	//立つ家庭のアニメーションが終わったらidleupdateに変更する
	if (animNo_ == anim_situpToIdle_no && PModel_->isAnimEnd()) {
		updateFunc_ = &Player::idleUpdate;
		isSitting_ = false;
		return;
	}

	//アニメーションを座る過程のアニメーションに変更
	//座っているフラグを立て、アニメーションループ変数を折る
	if (!isSitting_) {
		animNo_ = anim_idleToSitup_no;
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
		animNo_ = anim_situpToIdle_no;
		isAnimLoop_ = false;
		PModel_->changeAnimation(animNo_, isAnimLoop_, false, 20);
	}
}
