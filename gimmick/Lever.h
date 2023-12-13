#pragma once
#include "../util/ObjectData.h"
#include <memory>
#include <DxLib.h>

class Model;

enum class AnimType {
	on,
	off,
	max,
};

class Lever
{
public:
	//�R���X�g���N�^
	Lever(LoadObjectInfo info,VECTOR stopPos);
	//�f�X�g���N�^
	virtual ~Lever();

	//�X�V
	void Update();
	//�`��
	void Draw();

	//�Փ˔���
	bool CollCheck(VECTOR playerPos);

	//�G���x�[�^�̒�~�ʒu�̎擾
	VECTOR GetElevatorStopPoint() { return elevatorStopPosition_; }

	//���f���|�C���^�[�̎擾
	std::shared_ptr<Model> GetModelPointer() { return model_; }

	//���o�[�̋N��
	void OnAnimation();

	//���o�[�̒�~
	void OffAnimation();

	//���o�[���N�����Ă��邩�ǂ����擾
	bool IsOn() { return isOn_; }

private:

	bool isOn_ = false;							//���o�[���N�����Ă��邩�ǂ���

	VECTOR pos_ = {};							//�|�W�V����
	VECTOR elevatorStopPosition_ = {};			//�G���x�[�^�̒�~�|�W�V����

	std::shared_ptr<Model> model_;				//���f���N���X�̃X�}�[�g�|�C���^

};

