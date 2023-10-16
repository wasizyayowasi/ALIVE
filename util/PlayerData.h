#pragma once

enum class AnimType {
	idle,			//�ҋ@���
	run,			//����
	stairs,			//�K�i�����
	radder,			//���
	jump,			//�W�����v
	runningJump,	//����W�����v
	death,			//����W�����v
	walk,			//����
	idleToSitup,	//idle��Ԃ������
	situpToIdle,	//�����Ă����Ԃ���idle
	sit,			//�����Ă���
	clim,			//�o��
	stand,			//�����オ��
	carryWalking,	//�^�тȂ������
	carryRunning,	//�^�тȂ��珬����
	carry,			//�^��
	max
};

struct PlayerInfo {
	float jumpPower;
	float runningJumpPower;
	float rotSpeed;
	float walkSpeed;
	float runningSpeed;
	int animNo_[static_cast<int>(AnimType::max)];
};