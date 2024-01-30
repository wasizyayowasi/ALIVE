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
	Switch,
	PenetrationScaffld,
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
	TV,
	Book,
	TunnelWall,
	TunnelEntrance,
	Max,
};

struct LoadObjectInfo {
	std::string name;	//�I�u�W�F�N�g�̖��O
	VECTOR pos = {};	//�|�W�V����
	VECTOR rot = {};	//��]��
	VECTOR scale = {};	//�g�k��
};