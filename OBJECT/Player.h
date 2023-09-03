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

	float movingSpeed_ = 5.0f;				//移動速度
	VECTOR playerPos_ = { 0,0,0 };			//プレイヤーのポジション

};

