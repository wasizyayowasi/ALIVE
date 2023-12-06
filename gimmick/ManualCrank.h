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
	ManualCrank(LoadObjectInfo objInfo);
	virtual ~ManualCrank();

	//�`��
	void Draw();

	//�v���C���[�ƃ��f���̓����蔻����s��
	bool HitCollPlayer(Player& player);

	//���f���|�C���^���擾
	std::shared_ptr<Model>GetModelPointer() { return model_; }

	VECTOR GetInitPos() { return initPos_; }

	//���݂�Z���̉�]�����擾����
	float GetRotZ() { return rotZ_; }

	//Z���̍ő��]�����擾����
	float GetMaxRotZ();

	//��]����ݒ肷��
	void SetRotZ(float rotZ) { rotZ_ = rotZ; }


	VECTOR GetStandingPosition() { return standingPos_; }

private:

	float rotZ_ = 0.0f;
	float radian_ = 0.0f;

	VECTOR pos_ = {};
	VECTOR initPos_ = {};
	VECTOR standingPos_ = {};

	std::shared_ptr<Model> model_;

};

