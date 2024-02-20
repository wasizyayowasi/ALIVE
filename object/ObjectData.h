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
	//生物類
	Enemy,
	Player,
	Corpse,
	CorpseMountain,
	SignBoardEnemy,

	//ギミック類
	Lever,
	Trans,
	Switch,
	Elevator,
	HopStepJump,
	CrankScaffold,
	PenetrationScaffld,

	//部屋に使う
	TV,
	Bed,
	Door,
	Book,
	Desk,
	Clock,
	Window,
	RoomWall,
	LightBulb,
	WoodenBox,
	WoodFloor,
	SignBoardType2,

	//メインステージに使う
	Tile,
	Fence,
	Train,
	Street,
	StoreC,
	TStreet,
	Station,
	WallBack,
	WallSide,
	TileType2,
	BuildingC,
	Building2A,
	TunnelWall,
	BigBuildingA,
	RedContainer,
	BlueContainer,
	SignBoardType1,
	TunnelEntrance,
	YellowContainer,
	OrangeContainer,

	//その他
	Stone,
	Number,
	WhiteBoard,

	Max,
};

const char* const ObjectName[static_cast<int>(ObjectType::Max)]
{
	//生物類
	"Enemy",
	"Player",
	"Corpse",
	"CorpseMountain",
	"SignBoardEnemy",

	//ギミック類
	"Lever",
	"Trans",
	"Switch",
	"Elevator",
	"HopStepJump",
	"CrankScaffold",
	"PenetrationScaffld",

	//部屋に使う
	"TV",
	"Bed",
	"Door",
	"Book",
	"Desk",
	"Clock",
	"Window",
	"RoomWall",
	"LightBulb",
	"WoodenBox",
	"WoodFloor",
	"SignBoardType2",

	//メインステージに使う
	"Tile",
	"Fence",
	"Train",
	"Street",
	"StoreC",
	"TStreet",
	"Station",
	"WallBack",
	"WallSide",
	"TileType2",
	"BuildingC",
	"Building2A",
	"TunnelWall",
	"BigBuildingA",
	"RedContainer",
	"BlueContainer",
	"SignBoardType1",
	"TunnelEntrance",
	"YellowContainer",
	"OrangeContainer",

	//その他
	"Stone",
	"Number",
	"WhiteBoard"
};

//計　76byte
struct LoadObjectInfo {
	std::string name;	//オブジェクトの名前	//40byte
	VECTOR pos = {};	//ポジション			//12byte
	VECTOR rot = {};	//回転率				//12byte
	VECTOR scale = {};	//拡縮率				//12byte
};