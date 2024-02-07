#pragma once

enum class PlayerAnimType {
	Idle,			//待機状態
	Walk,			//歩く
	Run,			//走る
	Jump,			//ジャンプ
	Crank,			//クランクを回す
	Death,			//死ぬ
	IdleToSitup,	//idle状態から座る
	SitupToIdle,	//座っている状態からidle
	LeverOn,		//レバーを起動する
	WakeUp,			//起き上がる
	Throw,			//投げる
	max
};

struct PlayerInfo {
	float jumpPower;
	float runningJumpPower;
	float rotSpeed;
	float walkSpeed;
	float runningSpeed;
};