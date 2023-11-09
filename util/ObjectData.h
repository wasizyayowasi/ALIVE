#pragma once
#include "DxLib.h"
#include <string>

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
	trans,
	elevator,
	max,
};

struct LoadObjectInfo {
	std::string name;	//オブジェクトの名前
	VECTOR pos = {};	//ポジション
	VECTOR rot = {};	//回転率
	VECTOR scale = {};	//拡縮率
};