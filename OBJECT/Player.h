#pragma once
#include "DxLib.h"

class InputState;

class Player
{
public:

	Player();
	virtual ~Player();

	void update(const InputState& input);
	void draw();

	VECTOR getPos() { return playerPos_; }

private:

	float movingSpeed_ = 5.0f;				//�ړ����x
	VECTOR playerPos_ = { 0,0,0 };			//�v���C���[�̃|�W�V����

};

