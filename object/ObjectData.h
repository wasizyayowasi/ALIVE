#pragma once
#include <string>
#include <DxLib.h>

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
	SignBoardPlayer,

	//ギミック類
	Lever,
	Switch,
	Elevator,
	HopStepJump,
	CrankScaffold,
	TransScaffold,
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
	Rock,
	Number,
	WhiteBoard,

	Max,
};

struct ObjectData
{
	ObjectType type = {};			//オブジェクトのタイプ
	std::string name = {};			//名前
	Material material = {};			//素材
	ObjectBaseType baseType = {};	//baseクラス
};

static ObjectData objData[static_cast<int>(ObjectType::Max)]
{
	//敵
	ObjectType::Enemy,
	"Enemy",
	Material::Other,
	ObjectBaseType::EnemyBase,

	//プレイヤー
	ObjectType::Player,
	"Player",
	Material::Other,
	ObjectBaseType::CharacterBase,

	//死体
	ObjectType::Corpse,
	"Corpse",
	Material::Other,
	ObjectBaseType::CharacterBase,

	//死体の山
	ObjectType::CorpseMountain,
	"CorpseMountain",
	Material::Other,
	ObjectBaseType::OrnamentBase,

	//看板の敵
	ObjectType::SignBoardPlayer,
	"SignBoardPlayer",
	Material::Other,
	ObjectBaseType::EnemyBase,


	//レバー
	ObjectType::Lever,
	"Lever",
	Material::Iron,
	ObjectBaseType::GimmickBase,

	//スイッチ
	ObjectType::Switch,
	"Switch",
	Material::Iron,
	ObjectBaseType::GimmickBase,

	//エレベーター
	ObjectType::Elevator,
	"Elevator",
	Material::Iron,
	ObjectBaseType::GimmickBase,

	//ホップステップジャンプ
	ObjectType::HopStepJump,
	"HopStepJump",
	Material::Other,
	ObjectBaseType::GimmickBase,

	//クランクの足場
	ObjectType::CrankScaffold,
	"CrankScaffold",
	Material::Iron,
	ObjectBaseType::GimmickBase,

	//透明な足場
	ObjectType::TransScaffold,
	"TransScaffold",
	Material::Iron,
	ObjectBaseType::GimmickBase,

	//貫通する足場
	ObjectType::PenetrationScaffld,
	"PenetrationScaffld",
	Material::Iron,
	ObjectBaseType::GimmickBase,

	//テレビ
	ObjectType::TV,
	"TV",
	Material::Iron,
	ObjectBaseType::OrnamentBase,

	//ベッド
	ObjectType::Bed,
	"Bed",
	Material::Wood,
	ObjectBaseType::OrnamentBase,

	//ドア
	ObjectType::Door,
	"Door",
	Material::Wood,
	ObjectBaseType::OrnamentBase,

	//本
	ObjectType::Book,
	"Book",
	Material::Other,
	ObjectBaseType::OrnamentBase,

	//机
	ObjectType::Desk,
	"Desk",
	Material::Wood,
	ObjectBaseType::OrnamentBase,

	//時計
	ObjectType::Clock,
	"Clock",
	Material::Iron,
	ObjectBaseType::OrnamentBase,

	//窓
	ObjectType::Window,
	"Window",
	Material::Wood,
	ObjectBaseType::OrnamentBase,

	//部屋の壁
	ObjectType::RoomWall,
	"RoomWall",
	Material::Stone,
	ObjectBaseType::OrnamentBase,

	//電球
	ObjectType::LightBulb,
	"LightBulb",
	Material::Other,
	ObjectBaseType::OrnamentBase,

	//木の箱
	ObjectType::WoodenBox,
	"WoodenBox",
	Material::Wood,
	ObjectBaseType::OrnamentBase,

	//木の床
	ObjectType::WoodFloor,
	"WoodFloor",
	Material::Wood,
	ObjectBaseType::OrnamentBase,

	//看板タイプ2
	ObjectType::SignBoardType2,
	"SignBoardType2",
	Material::Iron,
	ObjectBaseType::OrnamentBase,

	//タイル
	ObjectType::Tile,
	"Tile",
	Material::Stone,
	ObjectBaseType::OrnamentBase,

	//フェンス
	ObjectType::Fence,
	"Fence",
	Material::Iron,
	ObjectBaseType::OrnamentBase,

	//電車
	ObjectType::Train,
	"Train",
	Material::Iron,
	ObjectBaseType::OrnamentBase,

	//道
	ObjectType::Street,
	"Street",
	Material::Stone,
	ObjectBaseType::OrnamentBase,

	//店C
	ObjectType::StoreC,
	"StoreC",
	Material::Iron,
	ObjectBaseType::OrnamentBase,

	//T型の道
	ObjectType::TStreet,
	"TStreet",
	Material::Stone,
	ObjectBaseType::OrnamentBase,

	//駅
	ObjectType::Station,
	"Station",
	Material::Iron,
	ObjectBaseType::OrnamentBase,

	//後ろの壁
	ObjectType::WallBack,
	"WallBack",
	Material::Stone,
	ObjectBaseType::OrnamentBase,

	//壁の横
	ObjectType::WallSide,
	"WallSide",
	Material::Stone,
	ObjectBaseType::OrnamentBase,

	//タイルタイプ2
	ObjectType::TileType2,
	"TileType2",
	Material::Stone,
	ObjectBaseType::OrnamentBase,

	//建物C
	ObjectType::BuildingC,
	"BuildingC",
	Material::Iron,
	ObjectBaseType::OrnamentBase,

	//建物A
	ObjectType::Building2A,
	"Building2A",
	Material::Iron,
	ObjectBaseType::OrnamentBase,

	//トンネルの壁
	ObjectType::TunnelWall,
	"TunnelWall",
	Material::Stone,
	ObjectBaseType::OrnamentBase,

	//でかい建物
	ObjectType::BigBuildingA,
	"BigBuildingA",
	Material::Iron,
	ObjectBaseType::OrnamentBase,

	//赤いコンテナ
	ObjectType::RedContainer,
	"RedContainer",
	Material::Iron,
	ObjectBaseType::OrnamentBase,

	//青いコンテナ
	ObjectType::BlueContainer,
	"BlueContainer",
	Material::Iron,
	ObjectBaseType::OrnamentBase,

	//看板のタイプ1
	ObjectType::SignBoardType1,
	"SignBoardType1",
	Material::Iron,
	ObjectBaseType::OrnamentBase,

	//トンネルの入口
	ObjectType::TunnelEntrance,
	"TunnelEntrance",
	Material::Stone,
	ObjectBaseType::OrnamentBase,

	//イエローコンテナ
	ObjectType::YellowContainer,
	"YellowContainer",
	Material::Iron,
	ObjectBaseType::OrnamentBase,

	//オレンジコンテナ
	ObjectType::OrangeContainer,
	"OrangeContainer",
	Material::Iron,
	ObjectBaseType::OrnamentBase,

	//岩
	ObjectType::Rock,
	"Rock",
	Material::Stone,
	ObjectBaseType::OrnamentBase,

	//数字
	ObjectType::Number,
	"Number",
	Material::Other,
	ObjectBaseType::OrnamentBase,

	//ホワイトボード
	ObjectType::WhiteBoard,
	"WhiteBoard",
	Material::Other,
	ObjectBaseType::OrnamentBase,
};

//計　76byte
struct LoadObjectInfo {
	std::string name;	//オブジェクトの名前	//40byte
	VECTOR pos = {};	//ポジション			//12byte
	VECTOR rot = {};	//回転率				//12byte
	VECTOR scale = {};	//拡縮率				//12byte
};