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
	carry,			//�^��
	max
};
