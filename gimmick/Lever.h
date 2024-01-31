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
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="info">�z�u�f�[�^</param>
	/// <param name="stopPos">�G���x�[�^�[�̒�~�ʒu</param>
	Lever(LoadObjectInfo info,VECTOR stopPos);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Lever();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �Փ˔���
	/// </summary>
	/// <param name="playerPos">�v���C���[�̃|�W�V����</param>
	/// <returns>true : �������Ă���  false : �������Ă��Ȃ�</returns>
	bool CollCheck(VECTOR playerPos);

	/// <summary>
	/// �G���x�[�^�̒�~�ʒu�̎擾
	/// </summary>
	/// <returns>�G���x�[�^�̒�~�ʒu</returns>
	VECTOR GetElevatorStopPoint() { return elevatorStopPosition_; }

	/// <summary>
	/// ���f���|�C���^�̎擾
	/// </summary>
	/// <returns>���f���̃|�C���^</returns>
	std::shared_ptr<Model> GetModelPointer() { return model_; }
	
	/// <summary>
	/// ���o�[�������|�W�V�����̎擾
	/// </summary>
	/// <returns>���o�[�������|�W�V����</returns>
	VECTOR GetStandingPosition() { return standingPos_; }

	/// <summary>
	/// ���o�[�̋N��
	/// </summary>
	void OnAnimation();

	/// <summary>
	/// ���o�[�̒�~
	/// </summary>
	void OffAnimation();

	/// <summary>
	/// ���o�[���N�����Ă��邩�ǂ����擾
	/// </summary>
	/// <returns>���o�[���N�����Ă��邩</returns>
	bool IsOn() { return isOn_; }
private:

	bool isOn_ = false;							//���o�[���N�����Ă��邩�ǂ���

	VECTOR pos_ = {};							//�|�W�V����
	VECTOR elevatorStopPosition_ = {};			//�G���x�[�^�̒�~�|�W�V����
	VECTOR standingPos_ = {};					//�G���x�[�^�̒�~�|�W�V����

	std::shared_ptr<Model> model_;				//���f���N���X�̃X�}�[�g�|�C���^

};

