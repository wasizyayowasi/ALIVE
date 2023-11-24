#pragma once

enum class AnimType {
	idle,			//待機状態
	walk,			//歩く
	run,			//走る
	jump,			//ジャンプ
	death,			//死ぬ
	idleToSitup,	//idle状態から座る
	carryWalking,	//運びながら歩く
	carryIdle,		//運ぶ
	dead,			//死ぬ
	situpToIdle,	//座っている状態からidle
	radder,			//上る
	runningJump,	//走りジャンプ
	sit,			//座っている
	clim,			//登る
	stand,			//立ち上がる
	carryRunning,	//運びながら小走り
	max
};

//enum class AnimType {
//	idle,			//待機状態
//	run,			//走る
//	stairs,			//階段を上る
//	radder,			//上る
//	jump,			//ジャンプ
//	runningJump,	//走りジャンプ
//	death,			//走りジャンプ
//	walk,			//歩く
//	idleToSitup,	//idle状態から座る
//	situpToIdle,	//座っている状態からidle
//	sit,			//座っている
//	clim,			//登る
//	stand,			//立ち上がる
//	carryWalking,	//運びながら歩く
//	carryRunning,	//運びながら小走り
//	carry,			//運ぶ
//	max
//};

struct PlayerInfo {
	float jumpPower;
	float runningJumpPower;
	float rotSpeed;
	float walkSpeed;
	float runningSpeed;
	int animNo_[static_cast<int>(AnimType::max)];
};