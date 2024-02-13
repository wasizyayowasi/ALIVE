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
	std::string name;	//�I�u�W�F�N�g�̖��O
	VECTOR pos = {};	//�|�W�V����
	VECTOR rot = {};	//��]��
	VECTOR scale = {};	//�g�k��
};