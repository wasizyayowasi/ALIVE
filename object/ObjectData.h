#pragma once
#include <DxLib.h>
#include <string>

//�I�u�W�F�N�g�ׁ̂[�X
enum class ObjectBaseType {
	CharacterBase,			//�L�����N�^�[�x�[�X
	EnemyBase,				//�G�x�[�X
	OrnamentBase,			//�u���x�[�X
	CarryBase,				//�^�ׂ�I�u�W�F�N�g�x�[�X
	GimmickBase,			//���u�x�[�X
};

//�I�u�W�F�N�g�̍ޗ�(������)
enum class Material {
	Stone,					//��
	Iron,					//�S
	Wood,					//��
	Other,					//���̑�
	max,
};

//�I�u�W�F�N�g�̃^�C�v
enum class ObjectType{
	//������
	Enemy,
	Player,
	Corpse,
	CorpseMountain,
	SignBoardEnemy,

	//�M�~�b�N��
	Lever,
	Trans,
	Switch,
	Elevator,
	HopStepJump,
	CrankScaffold,
	PenetrationScaffld,

	//�����Ɏg��
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

	//���C���X�e�[�W�Ɏg��
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

	//���̑�
	Stone,
	Number,
	WhiteBoard,

	Max,
};

const char* const ObjectName[static_cast<int>(ObjectType::Max)]
{
	//������
	"Enemy",
	"Player",
	"Corpse",
	"CorpseMountain",
	"SignBoardEnemy",

	//�M�~�b�N��
	"Lever",
	"Trans",
	"Switch",
	"Elevator",
	"HopStepJump",
	"CrankScaffold",
	"PenetrationScaffld",

	//�����Ɏg��
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

	//���C���X�e�[�W�Ɏg��
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

	//���̑�
	"Stone",
	"Number",
	"WhiteBoard"
};

//�v�@76byte
struct LoadObjectInfo {
	std::string name;	//�I�u�W�F�N�g�̖��O	//40byte
	VECTOR pos = {};	//�|�W�V����			//12byte
	VECTOR rot = {};	//��]��				//12byte
	VECTOR scale = {};	//�g�k��				//12byte
};