#pragma once
#include "DxLib.h"
#include <memory>

class Model;
class InputState;
class Player;

enum class State {
	top,
	left,
	bottom,
	right,
};

class ManualCrank
{
public:
	ManualCrank();
	virtual ~ManualCrank();

	void Update(const InputState& input);
	void Draw();

	//��]�̍X�V
	void AngleUpdate(const InputState& input);

	//�����ݎg���ĂȂ���
	//���݂̏�Ԃ��p�x�ɂ���ĕω�������
	void CurrentStateUpdate();

	//���Ɉړ��ł���p�x�̏�Ԃ��X�V����
	void NextStateUpdate();

	//�v���C���[�ƃ��f���̓����蔻����s��
	bool HitCollPlayer(Player& player);

	std::shared_ptr<Model>GetModelPointer() { return model_; }

	//���݂�Z���̉�]�����擾����
	float GetRotZ() { return rotZ_; }

	//Z���̍ő��]�����擾����
	float GetMaxRotZ();

private:

	int crankLaps_ = 0;

	float rotZ_ = 0.0f;

	VECTOR pos_ = { -200, 100, 0 };
	VECTOR rot_ = {};

	State currentState_ = {};

	State turnRight_ = {};
	State turnLeft_ = {};

	std::shared_ptr<Model> model_;

};

