#include "Camera.h"
#include "../util/InputState.h"
#include <algorithm>

namespace {
	constexpr float horizonta_size_of_one_room = 1000.0f;

	constexpr float camera_sride_speed = 10.0f;

	constexpr float add_focus = 50.0f;
}

Camera::Camera()
{
	tempRoom[0] = 1000;
	tempRoom[1] = 1500;
	tempRoom[2] = 2000;

	threshold = tempRoom[0];
}

Camera::~Camera()
{
}

void Camera::init()
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

//�v���C���[��ǐՂ���J�����̍X�V
void Camera::trackingCameraUpdate(VECTOR playerPos)
{
	
	i = (std::max)(i, 0);

	tracking(playerPos);

	//�J�������v���C���[��ǂ�������p�ɂ���
	cameraPos_.y = ((300.0f * 0.9f) + (playerPos.y * 0.1f));
	cameraPos_.z = playerPos.z - 800.0f;

	//�v���C���[�������ʒu������悤�ɂ���
	cameraTarget_.x = (cameraTarget_.x * 0.9f) + (playerPos.x * 0.1f);
	cameraTarget_.y = (cameraTarget_.y * 0.9f) + (playerPos.y * 0.1f);
	cameraTarget_.z = (cameraTarget_.z * 0.95f) + (playerPos.z * 0.05f);

	SetCameraPositionAndTarget_UpVecY(cameraPos_, cameraTarget_);

}

//��_�J�����̍X�V
void Camera::fixedPointCamera(VECTOR playerPos)
{
	//���͈͂��o����J���������̏ꏊ�փk�����Ɠ���

	//�v���C���[�����镔���̔ԍ����擾
	int playerRoomNum = (playerPos.x + horizonta_size_of_one_room / 2 ) / (horizonta_size_of_one_room) ;
	//�J���������镔���̔ԍ����擾����
	int cameraRoomNum = fixedPointCameraDestinationPosX / (horizonta_size_of_one_room);
	//�v���C���[�ƃJ���������镔���̔ԍ����������J������X���W�ϐ�(��)���v���C���[�����镔���̔ԍ��ɕς���
	if (playerRoomNum != cameraRoomNum) {
		fixedPointCameraDestinationPosX = horizonta_size_of_one_room * playerRoomNum;
	}

	//�J�����̖ڕW�ʒu����J�����̃|�W�V����������
	float sub = fixedPointCameraDestinationPosX - cameraPos_.x;

	//�J�����̃|�W�V�����ƖڕW�ʒu�������ł͂Ȃ�������
	if (cameraPos_.x != fixedPointCameraDestinationPosX) {
		//�J�����̃|�W�V�������ړ�������
		if (sub > 0.0f) {
			cameraPos_.x = (std::min)(cameraPos_.x + 20.0f, fixedPointCameraDestinationPosX);
		}
		else {
			cameraPos_.x = (std::max)(cameraPos_.x - 20.0f, fixedPointCameraDestinationPosX);
		}
	}

	//�J����������ʒu���v���C���[���班�����炷
	cameraTarget_.x = (cameraTarget_.x * 0.9f) + (playerPos.x * 0.1f);
	cameraTarget_.y = (cameraTarget_.y * 0.9f) + (playerPos.y * 0.1f);
	cameraTarget_.z = 0;

	DrawFormatString(0, 80, 0x448844, "x : %.2f,y : %.2f,z : %.2f", cameraPos_.x, cameraPos_.y, cameraPos_.z);

	SetCameraPositionAndTarget_UpVecY(cameraPos_, cameraTarget_);
}

//�J�����̒����_����炷
void Camera::changeOfFocus(const InputState& input)
{
	if (input.isPressed(InputType::upArrow)) {
		cameraTarget_.y += add_focus;
	}
	else if (input.isPressed(InputType::downArrow)) {
		cameraTarget_.y -= add_focus;
	}
	else if (input.isPressed(InputType::leftArrow)) {
		cameraTarget_.x -= add_focus;
	} 
	else if (input.isPressed(InputType::rightArrow)) {
		cameraTarget_.x += add_focus;
	}
}

//�v���C���[��ǐ�
void Camera::tracking(VECTOR playerPos)
{

	bool overRightEndOfTheRoom = playerPos.x > threshold;
	bool overLeftEndOfTheRoom = playerPos.x > threshold - tempRoom[i];

	bool belowRightEndOfTheRoom = playerPos.x < threshold;
	bool belowLeftEndOfTheRoom = playerPos.x < threshold - tempRoom[i];


	//������̃��C���ɗ���ƃJ�����̈ʒu���E����(x���v���X�̕���)�Ɉړ�������
	if (overRightEndOfTheRoom && overLeftEndOfTheRoom) {
		cameraPos_.x = (std::min)(cameraPos_.x + camera_sride_speed, threshold + 200.0f);
		if (playerPos.x >= threshold + 50.0f) {
			i++;
			threshold += tempRoom[i];
		}
	}
	//������̃��C���ɗ���ƃJ�����̈ʒu��������(x���}�C�i�X�̕���)�Ɉړ�������
	else if (belowRightEndOfTheRoom && belowLeftEndOfTheRoom) {
		if (playerPos.x < threshold - tempRoom[i] - 50.0f) {
			threshold -= tempRoom[i];
			i--;
		}
		if (cameraPos_.x > threshold - tempRoom[i] - 200.0f) {
			cameraPos_.x -= camera_sride_speed;
		}
	}
	//�ʏ펞�̃J�����ړ�
	else {
		cameraPos_.x = (cameraPos_.x * 0.9f) + (playerPos.x * 0.1f);
	}
}


