#pragma once
#include <DxLib.h>
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
	gimmickSwitch,
	gimmickSteelyard,
	trans,
	CrankScaffold,
	elevator,
	BigBuildingA,
	BigBuildingB,
	BigBuildingC,
	BigBuildingD,
	BuildingAType1,
	BuildingBType1,
	BuildingCType1,
	BuildingAType2,
	BuildingBType2,
	BuildingCType2,
	StoreA,
	StoreB,
	StoreC,
	Street,
	TStreet,
	Tile,
	Scaffold,
	BlueContainer,
	RedContainer,
	YellowContainer,
	OrangeContainer,
	max,
};

struct LoadObjectInfo {
	std::string name;	//オブジェクトの名前
	VECTOR pos = {};	//ポジション
	VECTOR rot = {};	//回転率
	VECTOR scale = {};	//拡縮率
};