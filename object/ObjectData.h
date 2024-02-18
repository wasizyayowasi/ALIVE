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
	Corpse,
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
	Lever,
	Max,
};

//計　76byte
struct LoadObjectInfo {
	std::string name;	//オブジェクトの名前	//40byte
	VECTOR pos = {};	//ポジション			//12byte
	VECTOR rot = {};	//回転率				//12byte
	VECTOR scale = {};	//拡縮率				//12byte
};