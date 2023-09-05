#include "Player.h"
#include "../UTIL/InputState.h"
#include "../UTIL/Model.h"

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
				playerPos_.z += movingSpeed_;
				animNo_ = anim_run_no;
				isMoving = true;
				model_->setRot({ 0.0f,180.0f * DX_PI_F / 180.0f,0.0f });
			}
			if (input.isPressed(InputType::down)) {
				playerPos_.z -= movingSpeed_;
				animNo_ = anim_run_no;
				isMoving = true;
				model_->setRot({ 0.0f,0.0f,0.0f });
			}
			if (input.isPressed(InputType::left)) {
				playerPos_.x -= movingSpeed_;
				animNo_ = anim_run_no;
				isMoving = true;
				model_->setRot({ 0.0f,90.0f * DX_PI_F / 180.0f,0.0f });
			}
			if (input.isPressed(InputType::right)) {
				playerPos_.x += movingSpeed_;
				animNo_ = anim_run_no;
				isMoving = true;
				model_->setRot({ 0.0f,270.0f * DX_PI_F / 180.0f,0.0f });
			}
		}
		
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
	
	model_->setPos(playerPos_);

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
			playerPos_.y += jumpVec_;
			if (playerPos_.y <= 16.0f) {
				jumpFlag_ = false;
			}
		}
	}

	//死亡
	{
		if (input.isPressed(InputType::death)) {
			DeadPlayer deadPerson;
			deadPerson.isEnable = true;
			deadPerson.deathPos = playerPos_;
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

	DrawSphere3D(playerPos_, 16, 32, 0x0000ff, 0x0000ff, true);

	for (const auto person : deadPlayer_) {
		if (person.isEnable) {
			DrawSphere3D(person.deathPos, 16, 32, 0xff0000, 0xff0000, true);
		}
	}
}
