#include "Player.h"
#include "../UTIL/InputState.h"
#include "../UTIL/Model.h"
#include "../UTIL/CheckCollitionModel.h"
#include<algorithm>

namespace {
	//アニメーション
	constexpr int anim_idle_no = 0;			//待機状態
	constexpr int anim_run_no = 1;			//走る
	constexpr int anim_stairs_no = 2;		//階段を上る
	constexpr int anim_clim_no = 3;			//上る
	constexpr int anim_jump_no = 4;			//ジャンプ
	constexpr int anim_runningJump_no = 5;	//走りジャンプ
	constexpr int anim_walk_no = 6;			//歩く

	//ジャンプ
	constexpr float jump_power = 15.0f;
	constexpr float gravity = -1.0f;

	//回転スピード
	constexpr float rot_speed = 15.0f;

	//ファイルパス
	const char* const filename = "DATA/player/player4.mv1";

	//プレイヤーサイズ
	const VECTOR player_scale = { 0.5f,0.5f ,0.5f };

	//初期プレイヤーの回転角度
	const VECTOR start_player_rot = { 0.0f,0.0f ,0.0f };

	//プレイヤーの高さ
	constexpr float player_hegiht = 50.0f;

	//移動スピード
	constexpr float walk_speed = 3.0f;
	constexpr float run_speed = 7.0f;
}

using namespace std;

Player::Player()
{
	model_ = make_shared<Model>(filename);
	model_->setAnimation(0, true, false);
	model_->setScale(player_scale);
	checkCollitionModel_ = make_shared<CheckCollitionModel>(std::shared_ptr<Player>(this));

	jump_.isJump = false;
	jump_.jumpVec = 0.0f;

}

Player::~Player()
{
}

void Player::update(const InputState& input, std::shared_ptr<Model> model)
{
	model_->update();

	moving(input);
	rotation();
	jump(input);
	death(input);
	idle();

	model_->setPos(pos_);

	checkCollitionModel_->checkCollition(moveVec_,model, player_hegiht,jump_.isJump,jump_.jumpVec);
}

void Player::draw()
{
	model_->draw();

	DrawSphere3D(pos_, 16, 32, 0x0000ff, 0x0000ff, true);
	DrawFormatString(0, 16, 0x448844, "targetAngle : %.2f", targetAngle_);
	DrawFormatString(0, 32, 0x448844, "rot_ : %.2f", rot_.y);
	DrawFormatString(0, 48, 0x448844, "tempAngle : %.2f", tempAngle_);

	for (const auto person : deadPlayer_) {
		if (person.isEnable) {
			DrawSphere3D(person.deathPos, 16, 32, 0xff0000, 0xff0000, true);
		}
	}
}

void Player::setJumpInfo(bool isJump, float jumpVec)
{
	jump_.isJump = isJump;
	jump_.jumpVec = jumpVec;
}

void Player::moving(const InputState& input)
{

	isMoving = false;
	moveVec_ = { 0.0f,0.0f,0.0f };
	movingSpeed_ = 0.0f;

	{
		if (input.isPressed(InputType::up)) {
			movingSpeed_ = walk_speed;
			moveVec_.z += movingSpeed_;
			animNo_ = anim_walk_no;
			isMoving = true;
			targetAngle_ = 180.0f;
		}
		if (input.isPressed(InputType::down)) {
			movingSpeed_ = walk_speed;
			moveVec_.z -= movingSpeed_;
			animNo_ = anim_walk_no;
			isMoving = true;
			targetAngle_ = 0.0f;
		}
		if (input.isPressed(InputType::left)) {
			movingSpeed_ = walk_speed;
			moveVec_.x -= movingSpeed_;
			animNo_ = anim_walk_no;
			isMoving = true;
			targetAngle_ = 90.0f;
		}
		if (input.isPressed(InputType::right)) {
			movingSpeed_ = walk_speed;
			moveVec_.x += movingSpeed_;
			animNo_ = anim_walk_no;
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
			targetAngle_ = -45.0f;
		}
		if ((input.isPressed(InputType::up) || input.isPressed(InputType::down) || input.isPressed(InputType::left) || input.isPressed(InputType::right)) && input.isPressed(InputType::shift)) {
			movingSpeed_ = run_speed;
			animNo_ = anim_run_no;
		}
		
		moveVec_ = VScale(VNorm(moveVec_),movingSpeed_);

		rotation();

		model_->changeAnimation(animNo_, true, false, 20);

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

void Player::jump(const InputState& input)
{
	//ジャンプ処理
	{
		if (!jump_.isJump) {
			if (input.isPressed(InputType::space)) {
				animNo_ = anim_jump_no;
				jump_.jumpVec += jump_power;
				jump_.isJump = true;
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

	model_->changeAnimation(animNo_, false, false, 20);

}

void Player::death(const InputState& input)
{
	//死亡
	{
		if (input.isTriggered(InputType::z)) {
			DeadPlayer deadPerson;
			deadPerson.isEnable = true;
			deadPerson.deathPos = pos_;
			deadPlayer_.push_back(deadPerson);

			deathNum = 0;
			for (const auto person : deadPlayer_) {
				if (person.isEnable) {
					deathNum++;
					if (deathNum > 9) {
						deadPlayer_.erase(deadPlayer_.begin());
						deathNum--;
					}
				}
			}
		}
	}
}

void Player::idle()
{
	//待機アニメーションに戻す
	if (!isMoving) {
		if (animNo_ == anim_walk_no || animNo_ == anim_run_no) {
			animNo_ = anim_idle_no;
			model_->changeAnimation(animNo_, true, false, 10);
		}
	}
}

void Player::rotation()
{
	differenceAngle_ = targetAngle_ - tempAngle_;
	//differenceAngle_ = targetAngle_ - rot_.y;
	if (differenceAngle_ >= 180.0f) {
		//differenceAngle_ = targetAngle_ - rot_.y - 360.0f;
		differenceAngle_ = targetAngle_ - tempAngle_ - 360.0f;
		//differenceAngle_ += rot_speed;
		//rot_.y += rot_speed;
	}
	else if (differenceAngle_ < 0.0f) {
		differenceAngle_ = targetAngle_ - tempAngle_ + 360.0f;
	}

	if (differenceAngle_ == 0.0f) {
	}
	else if (differenceAngle_ < 0.0f) {
		rot_.y -= rot_speed;
		tempAngle_ -= rot_speed;
	}
	else {
		rot_.y += rot_speed;
		tempAngle_ += rot_speed;
	}

	if (tempAngle_ == 360.0f || tempAngle_ == -360.0f) {
		tempAngle_ = 0.0f;
	}

	if (rot_.y == 360.0f || rot_.y == -360.0f) {
		rot_.y = 0.0f;
	}

	model_->setRot({ rot_.x,rot_.y * DX_PI_F / 180.0f,rot_.z });
}
