#pragma once
#include "DxLib.h"
#include <string>

enum class ObjectBaseType {
	characterBase,			//�L�����N�^�[�x�[�X
	enemyBase,				//�G�x�[�X
	ornamentBase,			//�u���x�[�X
	carryBase,				//�^�ׂ�I�u�W�F�N�g�x�[�X
	gimmickBase,			//���u�x�[�X
	temp,
};

enum class ObjectType{
	player,
	deadPerson,
	enemy,
	field,
	carry,
	gimmickSwitch,
	gimmickSteelyard,
	tempsofa,
	tempbed,
};

struct LoadObjectInfo {
	std::string name;	//�I�u�W�F�N�g�̖��O
	VECTOR pos;			//�|�W�V����
	VECTOR rot;			//��]��
	VECTOR scale;		//�g�k��
};