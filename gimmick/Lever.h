#pragma once
#include "../object/ObjectData.h"
#include <memory>
#include <DxLib.h>

class Model;

class Lever
{
private:

	enum class AnimType {
		on,
		off,
		max,
	};

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
	
	//���o�[�������|�W�V�����̎擾
	VECTOR GetStandingPosition() { return standingPos_; }

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
	VECTOR standingPos_ = {};					//�G���x�[�^�̒�~�|�W�V����

	std::shared_ptr<Model> model_;				//���f���N���X�̃X�}�[�g�|�C���^

};

