#pragma once
#include <DxLib.h>
#include <string>

enum class ObjectBaseType {
	CharacterBase,			//�L�����N�^�[�x�[�X
	EnemyBase,				//�G�x�[�X
	OrnamentBase,			//�u���x�[�X
	CarryBase,				//�^�ׂ�I�u�W�F�N�g�x�[�X
	GimmickBase,			//���u�x�[�X
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
	std::string name;	//�I�u�W�F�N�g�̖��O
	VECTOR pos = {};	//�|�W�V����
	VECTOR rot = {};	//��]��
	VECTOR scale = {};	//�g�k��
};