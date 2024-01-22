#pragma once
#include <DxLib.h>
#include <string>

enum class ObjectBaseType {
	CharacterBase,			//キャラクターベース
	EnemyBase,				//敵ベース
	OrnamentBase,			//置物ベース
	CarryBase,				//運べるオブジェクトベース
	GimmickBase,			//装置ベース
};

enum class ObjectType{
	Player,
	DeadPerson,
	Enemy,
	GimmickSwitch,
	PenetrationScaffld,
	Trans,
	CrankScaffold,
	Elevator,
	SignBoard,
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
	Bed,
	Chair,
	Clock,
	Desk,
	RoomWall,
	Window,
	WoodFloor,
	WoodenBox,
	Max,
};

struct LoadObjectInfo {
	std::string name;	//オブジェクトの名前
	VECTOR pos = {};	//ポジション
	VECTOR rot = {};	//回転率
	VECTOR scale = {};	//拡縮率
};