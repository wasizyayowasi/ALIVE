#include "Player.h"
#include "../UTIL/InputState.h"

namespace {
	constexpr float jump_power = 30.0f;
	constexpr float gravity = -1.0f;
}

Player::Player()
{
}

Player::~Player()
{
}

void Player::update(const InputState& input)
{
	//移動
	{
		if (input.isPressed(InputType::up)) {
			playerPos_.z += movingSpeed_;
		}
		if (input.isPressed(InputType::down)) {
			playerPos_.z -= movingSpeed_;
		}
		if (input.isPressed(InputType::left)) {
			playerPos_.x -= movingSpeed_;
		}
		if (input.isPressed(InputType::right)) {
			playerPos_.x += movingSpeed_;
		}

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

}

void Player::draw()
{
	DrawSphere3D(playerPos_, 16, 32, 0x0000ff, 0x0000ff, true);

	for (const auto person : deadPlayer_) {
		if (person.isEnable) {
			DrawSphere3D(person.deathPos, 16, 32, 0xff0000, 0xff0000, true);
		}
	}
}
