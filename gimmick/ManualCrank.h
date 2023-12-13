#pragma once
#include "../util/ObjectData.h"
#include <DxLib.h>
#include <memory>

class Model;
class InputState;
class Player;

class ManualCrank
{
public:
	//�R���X�g���N�^
	ManualCrank(LoadObjectInfo objInfo);
	//�f�X�g���N�^
	virtual ~ManualCrank();

	//�`��
	void Draw();

	//�v���C���[�ƃ��f���̓����蔻����s��
	bool HitCollPlayer(Player& player);

	//���f���|�C���^���擾
	std::shared_ptr<Model>GetModelPointer() { return model_; }

	//���݂�Z���̉�]�����擾����
	float GetRotZ() { return rotZ_; }

	//Z���̍ő��]�����擾����
	float GetMaxRotZ();

	//��]����ݒ肷��
	void SetRotZ(float rotZ) { rotZ_ = rotZ; }

	//�N�����N���񂷍ۂ̗����ʒu�̎擾
	VECTOR GetStandingPosition() { return standingPos_; }

private:

	float rotZ_ = 0.0f;					//Z���̉�]��
	float radian_ = 0.0f;				//���W�A��

	VECTOR pos_ = {};					//�|�W�V����
	VECTOR initPos_ = {};				//�����|�W�V����
	VECTOR standingPos_ = {};			//�N�����N���񂷗����ʒu

	std::shared_ptr<Model> model_;		//���f���N���X�̃X�}�[�g�|�C���^

};

