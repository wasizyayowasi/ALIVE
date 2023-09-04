#pragma once
#include "DxLib.h"
#include <vector>

class InputState;

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

	int deathNum = 0;						//���S��

	float movingSpeed_ = 5.0f;				//�ړ����x
	float jumpVec_ = 0.0f;					//�W�����v�x�N�g��

	bool jumpFlag_ = false;					//�W�����v���Ă��邩�ǂ����̃t���O
	bool tempBool = false;

	VECTOR playerPos_ = { 0,16,0 };			//�v���C���[�̃|�W�V����

	std::vector<DeadPlayer> deadPlayer_;	//���̂�ۑ����邽��

};

