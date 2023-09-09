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

	void update(const InputState& input, std::vector<std::shared_ptr<Model>> models);
	void draw();

	VECTOR getPos() { return pos_; }
	void setPos(VECTOR pos) { pos_ = pos; }
	JumpInfo getJumpInfo() { return jump_; }
	void setJumpInfo(bool isJump, float jumpVec);

private:
	//�ړ�����
	void movingUpdate(const InputState& input);
	//�W�����v����
	void jumpUpdate(const InputState& input);
	//���S����
	void death(const InputState& input);
	//�ҋ@����
	void changeAnimIdle();
	//��]����
	void rotationUpdate();

private:

	int animNo_ = 0;						//���݂̃A�j���[�V�����ԍ�
	int deathNum = 0;						//���S��

	float movingSpeed_ = 0.0f;				//�ړ����x
	float targetAngle_ = 0.0f;				//��]
	float differenceAngle_ = 0.0f;			//�ڕW�̊p�x�ƌ��݂̊p�x�̍�
	float tempAngle_ = 0.0f;

	bool isMoving = false;					//�ړ�����
	bool isDead_ = false;					//����ł��邩

	JumpInfo jump_;							//�W�����v�֘A�̍\����

	VECTOR pos_ = { 0.0f,0.0f,0.0f };		//�v���C���[�̃|�W�V����
	VECTOR rot_ = { 0.0f,0.0f,0.0f };		//�v���C���[�̉�]
	VECTOR moveVec_ = { 0.0f,0.0f,0.0f };	//�v���C���[�̈ړ��x�N�g��

	std::vector<std::shared_ptr<Model>> deadPlayer_;	//���̂�ۑ����邽��
	std::shared_ptr<Model> PModel_;
	std::shared_ptr<Model> cube_;
	std::vector<std::shared_ptr<Model>> models_;
	std::shared_ptr<CheckCollitionModel> checkCollitionModel_;
};

