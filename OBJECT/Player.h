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

	VECTOR getPos() { return playerPos_; }

private:

	int animNo_ = 0;						//���݂̃A�j���[�V�����ԍ�
	int deathNum = 0;						//���S��

	float movingSpeed_ = 5.0f;				//�ړ����x
	float jumpVec_ = 0.0f;					//�W�����v�x�N�g��

	bool jumpFlag_ = false;					//�W�����v���Ă��邩�ǂ����̃t���O
	bool tempBool = false;

	VECTOR playerPos_ = { 0,16,0 };			//�v���C���[�̃|�W�V����

	std::vector<DeadPlayer> deadPlayer_;	//���̂�ۑ����邽��
	std::shared_ptr<Model> model_;

};

