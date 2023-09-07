#pragma once
#include "DxLib.h"
#include <vector>
#include <memory>

class InputState;
class Model;
class CheckCollitionModel;

struct DeadPlayer {
	bool isEnable;
	VECTOR deathPos;
};

struct JumpInfo {
	float jumpVec;		//�W�����v�x�N�g��
	bool isJump;		//�W�����v�����ǂ���
};

class Player
{
public:

	Player();
	virtual ~Player();

	void update(const InputState& input,std::shared_ptr<Model> model);
	void draw();

	VECTOR getPos() { return pos_; }
	void setPos(VECTOR pos) { pos_ = pos; }
	JumpInfo getJumpInfo() { return jump_; }
	void setJumpInfo(bool isJump, float jumpVec);

private:
	//�ړ�����
	void moving(const InputState& input);
	//�W�����v����
	void jump(const InputState& input);
	//���S����
	void death(const InputState& input);
	//�ҋ@����
	void idle();
	//��]����
	void rotation();

private:

	int animNo_ = 0;						//���݂̃A�j���[�V�����ԍ�
	int deathNum = 0;						//���S��

	float movingSpeed_ = 5.0f;				//�ړ����x
	float targetAngle_ = 0.0f;				//��]
	float differenceAngle_ = 0.0f;			//�ڕW�̊p�x�ƌ��݂̊p�x�̍�
	float tempAngle_ = 0.0f;

	bool isMoving = false;					//�ړ�����

	JumpInfo jump_;							//�W�����v�֘A�̍\����

	VECTOR pos_ = { 0.0f,0.0f,0.0f };		//�v���C���[�̃|�W�V����
	VECTOR rot_ = { 0.0f,0.0f,0.0f };		//�v���C���[�̉�]
	VECTOR moveVec_ = { 0.0f,0.0f,0.0f };	//�v���C���[�̈ړ��x�N�g��

	std::vector<DeadPlayer> deadPlayer_;	//���̂�ۑ����邽��
	std::shared_ptr<Model> model_;
	std::shared_ptr<CheckCollitionModel> checkCollitionModel_;
};

