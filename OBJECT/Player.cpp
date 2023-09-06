#include "Player.h"
#include "../UTIL/InputState.h"
#include "../UTIL/Model.h"
#include<algorithm>

namespace {
	//アニメーション
	constexpr int anim_idle_no = 0;			//待機状態
	constexpr int anim_run_no = 1;			//走る
	constexpr int anim_stairs_no = 2;		//階段を上る

	//ジャンプ
	constexpr float jump_power = 30.0f;
	constexpr float gravity = -1.0f;

	//ファイルパス
	const char* const filename = "DATA/player/player.mv1";

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
	bool isMoving = false;			//移動中か
	

	model_->update();

	//移動
	{
		if (!tempBool) {
			if (input.isPressed(InputType::up)) {
				pos_.z += movingSpeed_;
				animNo_ = anim_run_no;
				isMoving = true;
				tempRot = 180.0f;
			}
			if (input.isPressed(InputType::down)) {
				pos_.z -= movingSpeed_;
				animNo_ = anim_run_no;
				isMoving = true;
				tempRot = 0.0f;
			}
			if (input.isPressed(InputType::left)) {
				pos_.x -= movingSpeed_;
				animNo_ = anim_run_no;
				isMoving = true;
				tempRot = 90.0f;
			}
			if (input.isPressed(InputType::right)) {
				pos_.x += movingSpeed_;
				animNo_ = anim_run_no;
				isMoving = true;
				tempRot = 270.0f;
			}
		}

		temp = tempRot - rot_.y / DX_PI_F * 180.0f;
		if (temp < 0.0f) {
			//temp += 10.0f;
			rot_.y = rot_.y + 10.0f * DX_PI_F / 180.0f;
		}
		else if(temp > 0.0f){
			//temp -= 10.0f;
			rot_.y = rot_.y - 10.0f * DX_PI_F / 180.0f;
		}
		//rot_.y = temp * DX_PI_F / 180.0f;

		model_->setRot(rot_);

		model_->changeAnimation(animNo_, true, false, 20);

		//デバッグ用
		/*{
			if (input.isPressed(InputType::next)) {
				playerPos_.y += movingSpeed_;
			}
			if (input.isPressed(InputType::prev)) {
				playerPos_.y -= movingSpeed_;
			}
		}*/
	}
	
	model_->setPos(pos_);

	//ジャンプ処理
	{
		if (!jumpFlag_) {
			if (input.isPressed(InputType::space)) {
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

	//死亡
	{
		if (input.isPressed(InputType::death)) {
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

	//待機アニメーションに戻す
	if (!isMoving) {
		if (animNo_ == anim_run_no) {
			animNo_ = anim_idle_no;
			model_->changeAnimation(animNo_, true, false, 10);
		}
	}

}

void Player::draw()
{
	model_->draw();

	DrawFormatString(0, 16, 0x0000ff, "temp:%f,rot:%f,player:%f", temp, tempRot, rot_.y);

	DrawSphere3D(pos_, 16, 32, 0x0000ff, 0x0000ff, true);

	for (const auto person : deadPlayer_) {
		if (person.isEnable) {
			DrawSphere3D(person.deathPos, 16, 32, 0xff0000, 0xff0000, true);
		}
	}
}
