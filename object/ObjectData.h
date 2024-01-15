#pragma once
#include <DxLib.h>
#include <string>

enum class ObjectBaseType {
	characterBase,			//�L�����N�^�[�x�[�X
	enemyBase,				//�G�x�[�X
	ornamentBase,			//�u���x�[�X
	carryBase,				//�^�ׂ�I�u�W�F�N�g�x�[�X
	gimmickBase,			//���u�x�[�X
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
	std::string name;	//�I�u�W�F�N�g�̖��O
	VECTOR pos = {};	//�|�W�V����
	VECTOR rot = {};	//��]��
	VECTOR scale = {};	//�g�k��
};