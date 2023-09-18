#include "Camera.h"
#include "../util/InputState.h"
#include <algorithm>

namespace {
	constexpr float horizonta_size_of_one_room = 1000.0f;
}

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

void Camera::trackingCameraUpdate(const InputState& input,VECTOR playerPos)
{
	
	cameraPos_.x = (cameraPos_.x * 0.9f) + (playerPos.x * 0.1f);
	cameraPos_.y = ((300.0f * 0.9f) + (playerPos.y * 0.1f));
	cameraPos_.z = -800.0f;

	cameraTarget_.x = (cameraTarget_.x * 0.9f) + (playerPos.x * 0.1f);
	cameraTarget_.y = (cameraTarget_.y * 0.9f) + (playerPos.y * 0.1f);
	cameraTarget_.z = 0;

	//0changeOfFocus(input);

	SetCameraPositionAndTarget_UpVecY(cameraPos_, cameraTarget_);

	DrawFormatString(0, 80, 0x448844, "x : %.2f,y : %.2f,z : %.2f", cameraPos_.x, cameraPos_.y, cameraPos_.z);

}

void Camera::fixedPointCamera(VECTOR playerPos)
{
	//���͈͂��o����J���������̏ꏊ�փk�����Ɠ���

	int playerRoomNum = (playerPos.x + horizonta_size_of_one_room / 2 ) / (horizonta_size_of_one_room) ;
	int cameraRoomNum = fixedPointCameraDestinationPosX / (horizonta_size_of_one_room);
	if (playerRoomNum != cameraRoomNum) {
		fixedPointCameraDestinationPosX = horizonta_size_of_one_room * playerRoomNum;
	}

	float sub = fixedPointCameraDestinationPosX - cameraPos_.x;
	if (cameraPos_.x != fixedPointCameraDestinationPosX) {
		if (sub > 0.0f) {
			cameraPos_.x = (std::min)(cameraPos_.x + 20.0f, fixedPointCameraDestinationPosX);
		}
		else {
			cameraPos_.x = (std::max)(cameraPos_.x - 20.0f, fixedPointCameraDestinationPosX);
		}
	}

//	DrawFormatString(0, 16, 0xffffff, "player:%d,camera%d", playerRoomNum, cameraRoomNum);
//	DrawFormatString(0, 32, 0xffffff, "player:%.2f", playerPos.x);
//	DrawFormatString(0, 48, 0xffffff, "fiexd%.2f", fixedPointCameraDestinationPosX);
//	DrawFormatString(0, 64, 0xffffff, "sub%.2f", sub);

	/*if (playerPos.x <= -500) {
		fixedPointCameraDestinationPosX = -horizonta_size_of_one_room;
		cameraPos_.x = (std::max)(cameraPos_.x - 20.0f, fixedPointCameraDestinationPosX);
	}
	else if (playerPos.x >= 500) {
		fixedPointCameraDestinationPosX = horizonta_size_of_one_room;
		cameraPos_.x = (std::min)(cameraPos_.x + 20.0f, fixedPointCameraDestinationPosX);
	}
	else {
		fixedPointCameraDestinationPosX = 0;
		cameraPos_.x = (std::max)(cameraPos_.x - 20.0f, fixedPointCameraDestinationPosX);
	}*/

	SetCameraPositionAndTarget_UpVecY(cameraPos_, playerPos);
}

void Camera::changeOfFocus(const InputState& input)
{
	if (input.isPressed(InputType::up)) {
		cameraTarget_.y += 100.0f;
	}
	else if (input.isPressed(InputType::left)) {
		cameraTarget_.x -= 100.0f;
	} 
	else if (input.isPressed(InputType::right)) {
		cameraTarget_.x += 100.0f;
	}
	else if (input.isPressed(InputType::down)) {
		cameraTarget_.y -= 100.0f;
	}
}

