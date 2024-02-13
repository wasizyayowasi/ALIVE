#pragma once
#include <DxLib.h>
#include <string>

//オブジェクトのべース
enum class ObjectBaseType {
	CharacterBase,			//キャラクターベース
	EnemyBase,				//敵ベース
	OrnamentBase,			//置物ベース
	CarryBase,				//運べるオブジェクトベース
	GimmickBase,			//装置ベース
};

//オブジェクトの材料(見た目)
enum class Material {
	Stone,					//石
	Iron,					//鉄
	Wood,					//木
	Other,					//その他
	max,
};

//オブジェクトのタイプ
enum class ObjectType{
	Player,
	DeadPerson,
	Enemy,
	Switch,
	LightBulb,
	Door,
	PenetrationScaffld,
	HopStepJump,
	Trans,
	CrankScaffold,
	Elevator,
	SignBoardType1,
	SignBoardType2,
	BigBuildingA,
	BuildingCType1,
	BuildingAType2,
	Train,
	Station,
	Fence,
	CorpseMountain,
	WhiteBoard,
	StoreC,
	Street,
	TStreet,
	Tile,
	TileType2,
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
	TV,
	Book,
	TunnelWall,
	TunnelEntrance,
	WallBack,
	WallSide,
	BoardEnemy,
	Number,
	Max,
};

struct LoadObjectInfo {
	std::string name;	//オブジェクトの名前
	VECTOR pos = {};	//ポジション
	VECTOR rot = {};	//回転率
	VECTOR scale = {};	//拡縮率
};