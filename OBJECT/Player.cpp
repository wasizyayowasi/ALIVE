#include "Player.h"
#include "../UTIL/InputState.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::update(const InputState& input)
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
	if (input.isPressed(InputType::next)) {
		playerPos_.y += movingSpeed_;
	}
	if (input.isPressed(InputType::prev)) {
		playerPos_.y -= movingSpeed_;
	}

}

void Player::draw()
{
	DrawSphere3D(playerPos_, 16, 32, 0x0000ff, 0x0000ff, true);
}
