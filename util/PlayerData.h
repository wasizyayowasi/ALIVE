#pragma once

enum class PlayerAnimType {
	Idle,			//待機状態
	Walk,			//歩く
	Run,			//走る
	jump,			//ジャンプ
	crank,			//クランクを回す
	Death,			//死ぬ
	idleToSitup,	//idle状態から座る
	situpToIdle,	//座っている状態からidle
	max
};

struct PlayerInfo {
	float jumpPower;
	float runningJumpPower;
	float rotSpeed;
	float walkSpeed;
	float runningSpeed;
};