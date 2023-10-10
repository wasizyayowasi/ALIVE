#pragma once

enum class AnimType {
	idle,			//待機状態
	run,			//走る
	stairs,			//階段を上る
	radder,			//上る
	jump,			//ジャンプ
	runningJump,	//走りジャンプ
	death,			//走りジャンプ
	walk,			//歩く
	idleToSitup,	//idle状態から座る
	situpToIdle,	//座っている状態からidle
	sit,			//座っている
	clim,			//登る
	stand,			//立ち上がる
	carryWalking,	//運びながら歩く
	carry,			//運ぶ
	max
};
