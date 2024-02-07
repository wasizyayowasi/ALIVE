#pragma once

enum class PlayerAnimType {
	Idle,			//�ҋ@���
	Walk,			//����
	Run,			//����
	Jump,			//�W�����v
	Crank,			//�N�����N����
	Death,			//����
	IdleToSitup,	//idle��Ԃ������
	SitupToIdle,	//�����Ă����Ԃ���idle
	LeverOn,		//���o�[���N������
	WakeUp,			//�N���オ��
	Throw,			//������
	max
};

struct PlayerInfo {
	float jumpPower;
	float runningJumpPower;
	float rotSpeed;
	float walkSpeed;
	float runningSpeed;
};