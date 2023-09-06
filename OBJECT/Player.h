#pragma once
#include "DxLib.h"
#include <vector>
#include <memory>

class InputState;
class Model;

struct DeadPlayer {
	bool isEnable;
	VECTOR deathPos;
};

class Player
{
public:

	Player();
	virtual ~Player();

	void update(const InputState& input);
	void draw();

	VECTOR getPos() { return pos_; }

private:
	//�ړ�����
	void moving(const InputState& input);
	//�W�����v����
	void jump(const InputState& input);
	//���S����
	void death(const InputState& input);
	//�ҋ@����
	void idle();

private:

	int animNo_ = 0;						//���݂̃A�j���[�V�����ԍ�
	int deathNum = 0;						//���S��

	float movingSpeed_ = 5.0f;				//�ړ����x
	float jumpVec_ = 0.0f;					//�W�����v�x�N�g��
	float targetAngle_ = 0.0f;					//��]
	float angle_ = 0.0f;

	bool jumpFlag_ = false;					//�W�����v���Ă��邩�ǂ����̃t���O
	bool isMoving = false;					//�ړ�����

	VECTOR pos_ = { 0,16,0 };			//�v���C���[�̃|�W�V����
	VECTOR rot_ = { 0.0f,0.0f,0.0f };	//�v���C���[�̉�]

	std::vector<DeadPlayer> deadPlayer_;	//���̂�ۑ����邽��
	std::shared_ptr<Model> model_;
};

