#include "Camera.h"
#include <algorithm>

Camera::Camera()
{
	/////////////// 3D�֘A�̐ݒ� /////////////
	// Z�o�b�t�@���g�p����
	SetUseZBuffer3D(true);
	// Z�o�b�t�@�ւ̏������݂��s��
	SetWriteZBuffer3D(true);
	// �|���S���̗��ʂ�`�悵�Ȃ�
	SetUseBackCulling(true);

	//////////////// �J�����̐ݒ� //////////////////
	// �J��������ǂꂾ�����ꂽ�Ƃ���( Near )����A �ǂ��܂�( Far )�̂��̂�`�悷�邩��ݒ�
	SetCameraNearFar(5.0f, 2800.0f);
	// �J�����̈ʒu�A�ǂ������Ă��邩��ݒ肷��
	SetCameraPositionAndTarget_UpVecY(cameraPos_, VGet(0, 0, 0));
	// �J�����̎���p��ݒ�(���W�A��)
	SetupCamera_Perspective(60.0f * DX_PI_F / 180.0f);
}

Camera::~Camera()
{
}

void Camera::trackingCameraUpdate(VECTOR playerPos)
{
	cameraPos_ = playerPos;
	cameraPos_.y += 300.0f;
	cameraPos_.z -= 600.0f;

	SetCameraPositionAndTarget_UpVecY(cameraPos_, playerPos);
}

void Camera::fixedPointCamera(VECTOR playerPos)
{
	//���͈͂��o����J���������̏ꏊ�փk�����Ɠ���
	if (playerPos.x >= 500) {
		fixedPointCameraDestinationPosX = 1000;
		cameraPos_.x = (std::min)(cameraPos_.x + 20.0f, fixedPointCameraDestinationPosX);
	}
	else {
		fixedPointCameraDestinationPosX = 0;
		cameraPos_.x = (std::max)(cameraPos_.x - 20.0f, fixedPointCameraDestinationPosX);
	}

	SetCameraPositionAndTarget_UpVecY(cameraPos_, playerPos);
}

