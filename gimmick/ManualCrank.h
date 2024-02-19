#pragma once
#include "../object/ObjectData.h"
#include <DxLib.h>
#include <memory>

class Model;
class InputState;
class Player;

class ManualCrank
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="info">�z�u�f�[�^</param>
	ManualCrank(const LoadObjectInfo objInfo);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~ManualCrank();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �v���C���[�ƃ��f���̓����蔻����s��
	/// </summary>
	/// <param name="player">�v���C���[�̎Q��</param>
	/// <returns>�v���C���[�ƏՓ˂�����</returns>
	bool HitCollPlayer(Player& player) const;

	/// <summary>
	/// ���f���|�C���^���擾
	/// </summary>
	/// <returns>���f���̃|�C���^</returns>
	std::shared_ptr<Model>GetModelPointer() const { return model_; }

	/// <summary>
	/// ���݂�Z���̉�]�����擾����
	/// </summary>
	/// <returns>Z���̉�]��</returns>
	float GetRotZ() const { return rotZ_; }

	/// <summary>
	/// Z���̍ő��]�����擾����
	/// </summary>
	/// <returns>Z���̍ő��]��</returns>
	float GetMaxRotZ() const;

	/// <summary>
	/// ��]����ݒ肷��
	/// </summary>
	/// <param name="rotZ">Z���̉�]��</param>
	void SetRotZ(float rotZ) { rotZ_ = rotZ; }

	/// <summary>
	/// �N�����N���񂷍ۂ̗����ʒu�̎擾
	/// </summary>
	/// <returns>�|�W�V����</returns>
	VECTOR GetStandingPosition() const { return standingPos_; }

private:

	float rotZ_ = 0.0f;					//Z���̉�]��
	float radian_ = 0.0f;				//���W�A��

	VECTOR pos_ = {};					//�|�W�V����
	VECTOR initPos_ = {};				//�����|�W�V����
	VECTOR standingPos_ = {};			//�N�����N���񂷗����ʒu

	std::shared_ptr<Model> model_;		//���f���N���X�̃X�}�[�g�|�C���^

};

