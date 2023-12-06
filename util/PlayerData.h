#pragma once

enum class PlayerAnimType {
	Idle,			//�ҋ@���
	Walk,			//����
	Run,			//����
	jump,			//�W�����v
	crank,			//�N�����N����
	Death,			//����
	idleToSitup,	//idle��Ԃ������
	situpToIdle,	//�����Ă����Ԃ���idle
	max
};

struct PlayerInfo {
	float jumpPower;
	float runningJumpPower;
	float rotSpeed;
	float walkSpeed;
	float runningSpeed;
};