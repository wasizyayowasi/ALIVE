#include "Player.h"
#include "../UTIL/InputState.h"
#include "../UTIL/Model.h"
#include<algorithm>

namespace {
	//アニメーション
	constexpr int anim_idle_no = 0;			//待機状態
	constexpr int anim_run_no = 1;			//走る
	constexpr int anim_stairs_no = 2;		//階段を上る
	constexpr int anim_clim_no = 3;			//上る
	constexpr int anim_jump_no = 4;			//上る
	constexpr int anim_runningJump_no = 5;	//上る

	//ジャンプ
	constexpr float jump_power = 15.0f;
	constexpr float gravity = -1.0f;

	//回転スピード
	constexpr float rot_speed = 15.0f;

	//ファイルパス
	const char* const filename = "DATA/player/player3.mv1";

	//プレイヤーサイズ
	const VECTOR player_scale = { 0.5f,0.5f ,0.5f };
}

Player::Player()
{
	model_ = std::make_shared<Model>(filename);
	model_->setAnimation(0, true, false);
	model_->setScale(player_scale);
}

Player::~Player()
{
}

void Player::update(const InputState& input)
{
	model_->update();

	moving(input);
	jump(input);
	death(input);
	idle();

	model_->setPos(pos_);
}

void Player::draw()
{
	model_->draw();

	DrawSphere3D(pos_, 16, 32, 0x0000ff, 0x0000ff, true);
	DrawFormatString(0, 16, 0xffffff, "%.2f", targetAngle_);

	for (const auto person : deadPlayer_) {
		if (person.isEnable) {
			DrawSphere3D(person.deathPos, 16, 32, 0xff0000, 0xff0000, true);
		}
	}
}

void Player::moving(const InputState& input)
{

	isMoving = false;

	//移動
	{
		if (input.isPressed(InputType::up)) {
			pos_.z += movingSpeed_;
			animNo_ = anim_run_no;
			isMoving = true;
			targetAngle_ = 180.0f;
		}
		if (input.isPressed(InputType::down)) {
			pos_.z -= movingSpeed_;
			animNo_ = anim_run_no;
			isMoving = true;
			if (targetAngle_ == 270.0f || targetAngle_ == 360.0f) {
				targetAngle_ = 360.0f;
			}
			else {
				targetAngle_ = 0.0f;
			}
		}
		if (input.isPressed(InputType::left)) {
			pos_.x -= movingSpeed_;
			animNo_ = anim_run_no;
			isMoving = true;
			targetAngle_ = 90.0f;
		}
		if (input.isPressed(InputType::right)) {
			pos_.x += movingSpeed_;
			animNo_ = anim_run_no;
			isMoving = true;
			if (targetAngle_ == 0.0f || targetAngle_ == -90.0f) {
				targetAngle_ = -90.0f;
			}
			else {
				targetAngle_ = 270.0f;
			}
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
		

		angle_ = targetAngle_ - rot_.y;
		if (angle_ < 0.0f) {
			rot_.y -= rot_speed;
		}
		else if (angle_ > 0.0f) {
			rot_.y += rot_speed;
		}

		model_->setRot({rot_.x,rot_.y * DX_PI_F / 180.0f,rot_.z});

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
		if (!jumpFlag_) {
			if (input.isPressed(InputType::space)) {
				animNo_ = anim_jump_no;
				jumpVec_ += jump_power;
				jumpFlag_ = true;
			}
		}

		if (jumpFlag_) {
			jumpVec_ += gravity;
			pos_.y += jumpVec_;
			if (pos_.y <= 16.0f) {
				jumpFlag_ = false;
			}
		}
	}

	model_->changeAnimation(animNo_, false, false, 20);

}

void Player::death(const InputState& input)
{
	//死亡
	{
		if (input.isTriggered(InputType::death)) {
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
		if (animNo_ == anim_run_no) {
			animNo_ = anim_idle_no;
			model_->changeAnimation(animNo_, true, false, 20);
		}
	}
}
