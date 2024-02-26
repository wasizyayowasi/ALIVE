#include "LightBulb.h"

#include "../util/Model.h"
#include "../util/Easing.h"

#include <algorithm>

namespace {
	//������
	constexpr float total_time = 360.0f;
}

//�R���X�g���N�^
LightBulb::LightBulb(const int handle, const  LoadObjectInfo& info)
{
	//���f���̐ݒ�
	model_ = std::make_shared<Model>(handle,Material::Iron);
	model_->SetPos(info.pos);
	model_->SetRot(info.rot);
	model_->SetScale(info.scale);

	rot_ = info.rot;
}

//�f�X�g���N�^
LightBulb::~LightBulb()
{
}

//�X�V
void LightBulb::Update()
{
	//���Ԍv��
	elapsedTime_ = (std::min)(elapsedTime_ + 1,total_time);

	//�p�x�̎擾
	angle_ = Easing::InOutSine(elapsedTime_, total_time, targetAngle_, angle_);

	if (elapsedTime_ >= total_time / 4) {
		//�o�ߎ��Ԃ̃��Z�b�g
		elapsedTime_ = 0.0f;

		//�ڕW�p�x��ύX����
		targetAngle_ -= targetAngle_ * 2;
	}

	//Z�̉�]���ʓx�@�ɕύX����
	rot_.z = angle_ * DX_PI_F / 180.0f;

	//��]�̐ݒ�
	model_->SetRot(rot_);
}

//�`��
void LightBulb::Draw()
{
	model_->Draw();
}

//��]�x�N�g���̎擾
const VECTOR& LightBulb::GetRotVec() const
{
	MATRIX mtxRotZ = MGetRotZ(rot_.z);

	VECTOR frontVec = VTransform({ 0,-1,0 }, mtxRotZ);

	return frontVec;
}

//�t���[���̃|�W�V�����̎擾
const VECTOR& LightBulb::GetFramePos() const
{
	VECTOR pos = model_->GetFrameLocalPosition("LightBulb");
	return pos;
}
