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
	constexpr float jump_power = 15.0f;
	constexpr float gravity = -1.0f;

	//回転スピード
	constexpr float rot_speed = 15.0f;

	//ファイルパス
	const char* const player_Filename = "DATA/player/player9.mv1";
	

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

Player::Player()
{
	//プレイヤーモデルの生成
	PModel_ = make_shared<Model>(player_Filename);
	//アニメーションの設定
	PModel_->setAnimation(anim_idle_no, true, false);
	//プレイヤーの大きさの調整
	PModel_->setScale(player_scale);
	//マップやブロックなどの当たり判定の生成
	checkCollisionModel_ = make_shared<CheckCollisionModel>();

	jump_.isJump = false;
	jump_.jumpVec = 0.0f;

}

Player::~Player()
{
}

void Player::update(const InputState& input, std::vector<std::shared_ptr<Model>> models)
{

	moveVec_ = { 0.0f,0.0f,0.0f };
	movingSpeed_ = 0.0f;

	PModel_->update();
	for (auto& dead : deadPlayer_) {
		dead->update();
	}

	if (!isDead_) {
		movingUpdate(input);
		rotationUpdate();
		jumpUpdate(input);
		sitUpdate(input);
		changeAnimIdle();
		death(input);
	}
	else {
		if (PModel_->isAnimEnd()) {
			pos_ = checkPoint_;
			
			isDead_ = false;
		}
	}

	for (auto& deadPerson : deadPlayer_) {
		models.push_back(deadPerson);
	}
	
	PModel_->changeAnimation(animNo_, isAnimLoop_, false, 20);

	PModel_->setPos(pos_);

	checkCollisionModel_->checkCollision(*this,moveVec_, models, player_hegiht, jump_.isJump, jump_.jumpVec);
}

void Player::draw()
{
	PModel_->draw();
	for (auto& deadPlayer : deadPlayer_) {
		if (deadPlayer->getEnable()) {
			deadPlayer->draw();
		}
	}
	
	//cube_->draw();

	DrawSphere3D(pos_, 16, 32, 0x0000ff, 0x0000ff, true);
	for (auto& deadPlayer : deadPlayer_) {
		if (deadPlayer->getEnable()) {
			DrawFormatString(0, 96, 0x448844, "x:%.2f y:%.2f z:%.2f", deadPlayer->getPos().x, deadPlayer->getPos().y, deadPlayer->getPos().z);
		}
	}

	for (const auto person : deadPlayer_) {
		if (person->getEnable()) {
			DrawSphere3D(person->getPos(), 16, 32, 0xff0000, 0xff0000, true);
		}
	}
}

void Player::setJumpInfo(bool isJump, float jumpVec)
{
	jump_.isJump = isJump;
	jump_.jumpVec = jumpVec;
}

void Player::movingUpdate(const InputState& input)
{

	isMoving = false;
	

	{
		if (input.isPressed(InputType::up)) {
			movingSpeed_ = walk_speed;
			moveVec_.z += movingSpeed_;
			animNo_ = anim_walk_no;
			isAnimLoop_ = true;
			isMoving = true;
			targetAngle_ = 180.0f;
		}
		if (input.isPressed(InputType::down)) {
			movingSpeed_ = walk_speed;
			moveVec_.z -= movingSpeed_;
			animNo_ = anim_walk_no;
			isAnimLoop_ = true;
			isMoving = true;
			targetAngle_ = 0.0f;
		}
		if (input.isPressed(InputType::left)) {
			movingSpeed_ = walk_speed;
			moveVec_.x -= movingSpeed_;
			animNo_ = anim_walk_no;
			isAnimLoop_ = true;
			isMoving = true;
			targetAngle_ = 90.0f;
		}
		if (input.isPressed(InputType::right)) {
			movingSpeed_ = walk_speed;
			moveVec_.x += movingSpeed_;
			animNo_ = anim_walk_no;
			isAnimLoop_ = true;
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
			animNo_ = anim_run_no;
		}
		
		moveVec_ = VScale(VNorm(moveVec_),movingSpeed_);

		rotationUpdate();

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

void Player::jumpUpdate(const InputState& input)
{
	//ジャンプ処理
	{
		if (!jump_.isJump) {
			if (input.isPressed(InputType::space)) {
				if (input.isPressed(InputType::shift)) {
					animNo_ = anim_runningJump_no;
				}
				else {
					animNo_ = anim_jump_no;
				}
				jump_.jumpVec += jump_power;
				jump_.isJump = true;
				isAnimLoop_ = false;
			}
		}

		if (jump_.isJump) {
			jump_.jumpVec += gravity;
			pos_.y += jump_.jumpVec;
			if (pos_.y <= 0.0f) {
				jump_.isJump = false;
			}
		}
	}
}

void Player::death(const InputState& input)
{
	//死亡
	{
		if (input.isTriggered(InputType::death)) {
			deadPerson_.isEnable = true;
			deadPerson_.deathPos = pos_;

			deadPersonGenerater();

			animNo_ = anim_death_no;

			isAnimLoop_ = false;
			isDead_ = true;
			
			deadPlayer_.back()->changeAnimation(animNo_, false, false, 10);

		}
	}
}

void Player::changeAnimIdle()
{
	//待機アニメーションに戻す
	if (!isMoving) {
		if (animNo_ == anim_walk_no || animNo_ == anim_run_no||animNo_ == anim_death_no) {
			animNo_ = anim_idle_no;
			isAnimLoop_ = true;
		}
	}
}

//完成品だから今後いじらなくていいと思う
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

void Player::deadPersonGenerater()
{

	deadPlayer_.push_back(make_shared<Model>(PModel_->getModelHandle()));
	deadPlayer_.back()->setPos(deadPerson_.deathPos);
	deadPlayer_.back()->setScale(player_scale);
	deadPlayer_.back()->setRot({ rot_.x,rot_.y * DX_PI_F / 180.0f,rot_.z });
	deadPlayer_.back()->setUseCollision(true, false);

	deathNum = 0;
	for (const auto person : deadPlayer_) {
		if (person->getEnable()) {
			deathNum++;
			if (deathNum > 9) {
				deadPlayer_.erase(deadPlayer_.begin());
				deathNum--;
			}
		}
	}
}

void Player::sitUpdate(const InputState& input)
{
	if (input.isTriggered(InputType::ctrl)) {
		if (!isSitting_) {
			animNo_ = anim_idleToSitup_no;
			isSitting_ = true;
			isAnimLoop_ = false;
		}
		else {
			animNo_ = anim_situpToIdle_no;
			isSitting_ = false;
			isAnimLoop_ = false;
		}
		
	}

	if (animNo_ == anim_idleToSitup_no && PModel_->isAnimEnd()) {
		animNo_ = anim_sit_no;
	}

}
