#pragma once
#include <DxLib.h>
#include <memory>

class Model;
class InputState;
class Player;

class ManualCrank
{
public:
	ManualCrank();
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

private:

	float rotZ_ = 0.0f;

	VECTOR pos_ = { -200, 100, 0 };

	std::shared_ptr<Model> model_;

};

