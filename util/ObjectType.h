#pragma once

enum class ObjectBaseType {
	characterBase,			//キャラクターベース
	enemyBase,				//敵ベース
	ornamentBase,			//置物ベース
	carryBase,				//運べるオブジェクトベース
	gimmickBase,			//装置ベース
};

enum class ObjectType{
	player,
	deadPerson,
	enemy,
	field,
	carry,
	gimmickSwitch,
	gimmickSteelyard,
};