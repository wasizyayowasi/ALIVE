#include "Camera.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include <algorithm>

namespace {
	constexpr float horizonta_size_of_one_room = 1000.0f;

	//�J���������E���ɓ��B�����Ƃ��X���C�h�ړ�����X�s�[�h
	constexpr float camera_moveX_speed = 10.0f;
	constexpr float camera_moveZ_speed = 40.0f;
	constexpr float camera_moveY_speed = 40.0f;

	//�����_����炷�Ƃ��̂ǂ̂��炢�����_�����炷�̂�
	constexpr float add_focus = 30.0f;

	//�J�����̏����|�W�V����
	const VECTOR init_pos = VGet(0, 300, -600);

	//�J������Z���W���ړ�����ہA�{�[�_�[���C��
	constexpr float tracking_Z_borderline = 1000.0f;

	//�J������Y���W���ړ�����ہA�{�[�_�[���C�����L���Ȕ͈�
	constexpr float border_range = 100.0f;
}

Camera::Camera()
{

	cameraPos_ = init_pos;

	tempRoom[0] = 1000;
	tempRoom[1] = 1500;
	tempRoom[2] = 2000;

	threshold = tempRoom[0];

	cameraTargetPosZ = -600;
}

Camera::~Camera()
{
}

void Camera::Init()
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
	SetCameraNearFar(5.0f, 3300.0f);
	// �J�����̈ʒu�A�ǂ������Ă��邩��ݒ肷��
	SetCameraPositionAndTarget_UpVecY(cameraPos_, VGet(0, 0, 0));
	// �J�����̎���p��ݒ�(���W�A��)
	SetupCamera_Perspective(60.0f * DX_PI_F / 180.0f);

}

//�v���C���[��ǐՂ���J�����̍X�V
void Camera::TrackingCameraUpdate(VECTOR playerPos,float playerHeight)
{
	
	i = (std::max)(i, 0);

//	Tracking(playerPos);

	//�J�������v���C���[��ǂ�������p�ɂ���
	cameraPos_.x = TrackingPosX(playerPos);
	cameraPos_.y = TrackingPosY(playerPos,playerHeight);
	cameraPos_.z = TrackingPozZ(playerPos);

	//�v���C���[�������ʒu������悤�ɂ���
	cameraTarget_.x = (cameraTarget_.x * 0.9f) + (playerPos.x * 0.1f);
	cameraTarget_.y = (cameraTarget_.y * 0.9f) + ((playerPos.y + playerHeight / 2) * 0.1f);
	cameraTarget_.z = (cameraTarget_.z * 0.95f) + (playerPos.z * 0.05f);

	SetCameraPositionAndTarget_UpVecY(cameraPos_, cameraTarget_);
}

//��_�J�����̍X�V
void Camera::FixedPointCamera(VECTOR playerPos)
{
	//���͈͂��o����J���������̏ꏊ�փk�����Ɠ���

	//�v���C���[�����镔���̔ԍ����擾
	int playerRoomNum = static_cast<int>((playerPos.x + horizonta_size_of_one_room / 2 ) / (horizonta_size_of_one_room));
	//�J���������镔���̔ԍ����擾����
	int cameraRoomNum = static_cast<int>(fixedPointCameraDestinationPosX / (horizonta_size_of_one_room));
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
void Camera::ChangeOfFocus(const InputState& input)
{
	
	GetJoypadDirectInputState(DX_INPUT_PAD1, &input_);

	if (input_.Ry < 0 || input.IsPressed(InputType::upArrow)) {
		cameraTarget_.y += add_focus;
	}
	if (input_.Ry > 0 || input.IsPressed(InputType::downArrow)) {
		cameraTarget_.y -= add_focus;
	}
	if (input_.Rx < 0 || input.IsPressed(InputType::leftArrow)) {
		cameraTarget_.x -= add_focus;
	} 
	if (input_.Rx > 0 || input.IsPressed(InputType::rightArrow)) {
		cameraTarget_.x += add_focus;
	}
}

//�v���C���[��ǐ�
float Camera::TrackingPosX(VECTOR playerPos)
{

	float boderlinePosX = ExternalFile::GetInstance().GetCameraGimmickInfo(playerPos, "TrackingBorderlineX").pos.x;
	float gimmickPosX = ExternalFile::GetInstance().GetCameraGimmickInfo(playerPos, "TrackingX").pos.x;
	float distance = 0.0f;

	if (boderlinePosX <= 0) {
		return init_pos.x;
	}

	if (playerPos.x < gimmickPosX && playerPos.x > boderlinePosX - border_range) {
		distance = gimmickPosX - cameraPos_.x;
		moveVecX = distance / camera_moveY_speed;
		moveVecX = moveVecX * 0.96f;

		return cameraPos_.x + moveVecX;
	}

	return (cameraPos_.x * 0.9f) + (playerPos.x * 0.1f);

}

float Camera::TrackingPosY(VECTOR playerPos, float playerHeight)
{

	VECTOR boderlinePos = ExternalFile::GetInstance().GetCameraGimmickInfo(playerPos, "TrackingBorderlineY").pos;
	float gimmickPosY = ExternalFile::GetInstance().GetCameraGimmickInfo(playerPos, "TrackingY").pos.y;
	float distance = 0.0f;
	float playerHeadPosY = playerPos.y + playerHeight;

	if (boderlinePos.y <= 0) {
		return init_pos.y;
	}

	distance = boderlinePos.x - playerPos.x;

	distance = (std::max)(distance, -distance);

	if (distance < 1000.0f) {
		if (playerHeadPosY < boderlinePos.y + border_range && playerHeadPosY > boderlinePos.y - border_range) {
			distance = gimmickPosY - cameraPos_.y;
			moveVecY = distance / camera_moveY_speed;
			moveVecY = moveVecY * 0.96f;

			return cameraPos_.y + moveVecY;
		}
	}
	

	return (cameraPos_.y * 0.9f) + ((playerHeadPosY + playerHeight) * 0.1f);
}

float Camera::TrackingPozZ(VECTOR playerPos)
{
	
	float gimmickPosZ = ExternalFile::GetInstance().GetCameraGimmickInfo(playerPos, "TrackingZ").pos.z;
	float distance = 0.0f;

	if (gimmickPosZ <= 0) {
		return init_pos.z;
	}

	if (playerPos.z < gimmickPosZ) {
		distance = init_pos.z - cameraPos_.z;
		moveVecZ = distance / camera_moveZ_speed;
		moveVecZ = moveVecZ * 0.96f;
		return cameraPos_.z + moveVecZ;
	}

	float targetPosZ = (init_pos.z * 0.3f) + (playerPos.z * 0.7f);
	distance = targetPosZ - cameraPos_.z;

	distance = distance / camera_moveZ_speed;
	moveVecZ = distance * 0.96f;

	return cameraPos_.z + moveVecZ;

}

void Camera::tempDraW()
{
	DrawLine3D(VGet(0, 500.0f, 0.0f), VGet(3000.0f, 500.0f, 0.0f), 0xff0000);
	DrawFormatString(0, 16, 0x448844, "%.2f,%.2f,%.2f", cameraPos_.x, cameraPos_.y, cameraPos_.z);
}


