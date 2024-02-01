#include "Camera.h"
#include "../util/Util.h"
#include "../util/Easing.h"
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
	const VECTOR init_pos = VGet(0, 400, -800);

	//�J������Z���W���ړ�����ہA�{�[�_�[���C��
	constexpr float tracking_Z_borderline = 1000.0f;

	//�J������Y���W���ړ�����ہA�{�[�_�[���C�����L���Ȕ͈�
	constexpr float border_range = 100.0f;

	//�J�����̈ړ��ɂ����鍇�v����
	constexpr float total_time = 90.0f;

	constexpr VECTOR temp = { 0,0,1 };
}

Camera::Camera(VECTOR pos, VECTOR viewPos):updateFunc_(&Camera::TrackingCameraUpdate)
{
	pos_ = pos;

	initPos_ = pos;

	cameraViewingPos_ = viewPos;
}

Camera::~Camera()
{
}

void Camera::Init(VECTOR targetPos)
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
	SetCameraNearFar(5.0f, 5000.0f);
	//�J�����̃|�W�V�����A����ꏊ�A��̕�����ݒ�
	SetCameraPositionAndTargetAndUpVec(pos_, cameraViewingPos_, upVec_);
	// �J�����̎���p��ݒ�(���W�A��)
	SetupCamera_Perspective(60.0f * DX_PI_F / 180.0f);
}

void Camera::Update(VECTOR playerPos, float playerHeight)
{
	float distanceSize = 2000.0f;

	VECTOR fixedRangePos = ExternalFile::GetInstance().GetCameraGimmickInfo(playerPos, "FixedCameraRange").pos;

	float fixedRangeSize = VSize(fixedRangePos);

	if (fixedRangeSize > 0.0f) {
		
		distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(fixedRangePos,playerPos);

		distanceSize = (std::max)(distanceSize, -distanceSize);
	}

	if (distanceSize < 1500.0f) {
		updateFunc_ = &Camera::FixedPointCamera;
	}
	else {
		updateFunc_ = &Camera::TrackingCameraUpdate;
	}

	(this->*updateFunc_)(playerPos, playerHeight);
}

//�v���C���[��ǐՂ���J�����̍X�V
void Camera::TrackingCameraUpdate(VECTOR playerPos,float playerHeight)
{
	
	i = (std::max)(i, 0);

	//�J�������v���C���[��ǂ�������p�ɂ���
	pos_.x = TrackingPosX(playerPos);
	pos_.y = TrackingPosY(playerPos,playerHeight);
	pos_.z = TrackingPozZ(playerPos);

	//�v���C���[�������ʒu������悤�ɂ���
	cameraViewingPos_.x = (cameraViewingPos_.x * 0.9f) + (playerPos.x * 0.1f);
	cameraViewingPos_.y = (cameraViewingPos_.y * 0.9f) + ((playerPos.y + playerHeight / 2) * 0.1f);
	cameraViewingPos_.z = (cameraViewingPos_.z * 0.95f) + (playerPos.z * 0.05f);

	SetCameraPositionAndTarget_UpVecY(pos_, cameraViewingPos_);
}

//��_�J�����̍X�V
void Camera::FixedPointCamera(VECTOR playerPos, float playerHeight)
{
	VECTOR fixedRangePos = ExternalFile::GetInstance().GetCameraGimmickInfo(playerPos, "FixedCameraRange").pos;
	VECTOR fixedPos = ExternalFile::GetInstance().GetCameraGimmickInfo(fixedRangePos, "FixedCamera").pos;

	VECTOR distance = VSub(fixedPos, pos_);

	VECTOR normalise = VNorm(distance);

	VECTOR moveVec = VScale(VScale(normalise, 10.0f), 0.96f);

	float distanceSize = VSize(distance);

	if (distanceSize > 5.0f) {
		pos_ = VAdd(pos_, moveVec);
	}

	//�J����������ʒu���v���C���[���班�����炷
	cameraViewingPos_.x = (cameraViewingPos_.x * 0.9f) + (playerPos.x * 0.1f);
	cameraViewingPos_.y = (cameraViewingPos_.y * 0.9f);
	cameraViewingPos_.z = (cameraViewingPos_.z * 0.95f) + (playerPos.z * 0.05f);

	SetCameraPositionAndTarget_UpVecY(pos_, cameraViewingPos_);
}

//�J�����̒����_����炷
void Camera::ChangeOfFocus()
{
	//�Z�k��
	auto& input = InputState::GetInstance();

	GetJoypadDirectInputState(DX_INPUT_PAD1, &input_);

	if (input_.Ry < 0 || input.IsPressed(InputType::upArrow)) {
		cameraViewingPos_.y += add_focus;
	}
	if (input_.Ry > 0 || input.IsPressed(InputType::downArrow)) {
		cameraViewingPos_.y -= add_focus;
	}
	if (input_.Rx < 0 || input.IsPressed(InputType::leftArrow)) {
		cameraViewingPos_.x -= add_focus;
	} 
	if (input_.Rx > 0 || input.IsPressed(InputType::rightArrow)) {
		cameraViewingPos_.x += add_focus;
	}

	SetCameraPositionAndTarget_UpVecY(pos_, cameraViewingPos_);
}

void Camera::EasingMoveCamera()
{
	//���Ԃ̍X�V
	elapsedTime_ = (std::min)(elapsedTime_ + 1.0f, total_time);

	//�J�����̍��W�ړ�
	pos_.x = Easing::InOutCubic(elapsedTime_, total_time, cameraTargetPos_.x, pos_.x);
	pos_.y = Easing::InOutCubic(elapsedTime_, total_time, cameraTargetPos_.y, pos_.y);
	pos_.z = Easing::InOutCubic(elapsedTime_, total_time, cameraTargetPos_.z, pos_.z);

	//�J�����̌���ʒu�̈ړ�
	cameraViewingPos_.x = Easing::InOutCubic(elapsedTime_, total_time, cameraTargetViewPos_.x, cameraViewingPos_.x);
	cameraViewingPos_.y = Easing::InOutCubic(elapsedTime_, total_time, cameraTargetViewPos_.y, cameraViewingPos_.y);
	cameraViewingPos_.z = Easing::InOutCubic(elapsedTime_, total_time, cameraTargetViewPos_.z, cameraViewingPos_.z);

	//�J�����̏������ψڂ�����
	upVec_.x = Easing::InOutCubic(elapsedTime_, total_time, targetUpVec_.x, upVec_.x);
	upVec_.y = Easing::InOutCubic(elapsedTime_, total_time, targetUpVec_.y, upVec_.y);
	upVec_.z = Easing::InOutCubic(elapsedTime_, total_time, targetUpVec_.z, upVec_.z);

	//�J�����̏��̍X�V
	SetCameraPositionAndTargetAndUpVec(pos_, cameraViewingPos_, upVec_);

	if (elapsedTime_ == total_time) {
		isMoving_ = false;
	}
	else {
		isMoving_ = true;
	}
}

void Camera::SetCameraTargetPosAndView(VECTOR targetPos, VECTOR targetViewPos, VECTOR upVec)
{
	//�ڕW�ʒu�̐ݒ�
	cameraTargetPos_ = targetPos;

	//����ʒu�̐ݒ�
	cameraTargetViewPos_ = targetViewPos;

	//�J�����̏����
	targetUpVec_ = upVec;

	//�o�ߎ��Ԃ����Z�b�g����
	elapsedTime_ = 0.0f;
}

void Camera::tempdraw()
{
	DrawFormatString(0, 16, 0xffffff, "�|�W�V�����@�@%f:%f:%f", pos_.x, pos_.y, pos_.z);
	DrawFormatString(0, 32, 0xffffff, "���Ă���ꏊ�@%f:%f:%f", cameraViewingPos_.x, cameraViewingPos_.y, cameraViewingPos_.z);

	DrawSphere3D(pos_, 16, 32, 0x0000ff, 0x0000ff, true);
}

//�v���C���[��ǐ�
float Camera::TrackingPosX(VECTOR playerPos)
{

	float boderlinePosX = ExternalFile::GetInstance().GetCameraGimmickInfo(playerPos, "TrackingBorderlineX").pos.x;
	float gimmickPosX = ExternalFile::GetInstance().GetCameraGimmickInfo(playerPos, "TrackingX").pos.x;
	float distance = 0.0f;

	if (playerPos.x < gimmickPosX && playerPos.x > boderlinePosX - border_range) {
		distance = gimmickPosX - pos_.x;
		moveVecX = distance / camera_moveY_speed;
		moveVecX = moveVecX * 0.96f;

		return pos_.x + moveVecX;
	}

	//return (pos_.x * 0.99f) + (playerPos.x * 0.01f);
	return (pos_.x * 0.9f) + (playerPos.x * 0.1f);

}

float Camera::TrackingPosY(VECTOR playerPos, float playerHeight)
{

	VECTOR boderlinePos = ExternalFile::GetInstance().GetCameraGimmickInfo(playerPos, "TrackingBorderlineY").pos;
	float gimmickPosY = ExternalFile::GetInstance().GetCameraGimmickInfo(playerPos, "TrackingY").pos.y;
	float distance = 0.0f;
	float playerHeadPosY = playerPos.y + playerHeight;

	distance = boderlinePos.x - playerPos.x;

	distance = (std::max)(distance, -distance);

	if (distance < 1000.0f) {
		if (playerHeadPosY < boderlinePos.y + border_range && playerHeadPosY > boderlinePos.y - border_range) {
			distance = gimmickPosY - pos_.y;
			moveVecY = distance / camera_moveY_speed;
			moveVecY = moveVecY * 0.96f;

			return pos_.y + moveVecY;
		}
	}
	

	return (pos_.y * 0.9f) + ((playerHeadPosY + playerHeight * 2) * 0.1f);
}

float Camera::TrackingPozZ(VECTOR playerPos)
{
	
	VECTOR gimmickPos = ExternalFile::GetInstance().GetCameraGimmickInfo(playerPos, "TrackingZ").pos;
	VECTOR distance = {};

	if (gimmickPos.z <= 0) {
		return init_pos.z;
	}

	distance = VSub(gimmickPos,playerPos);
	float distanceSize = VSize(distance);

	if (distanceSize < 1500.0f) {
		if (playerPos.z > gimmickPos.z) {
			float targetPosZ = (init_pos.z * 0.3f) + (playerPos.z * 0.7f);
			distance.x = targetPosZ - pos_.z;

			distance.x = distance.x / camera_moveZ_speed;
			moveVecZ = distance.x * 0.96f;

			return pos_.z + moveVecZ;
		}
	}

	distance.z = init_pos.z - pos_.z;
	moveVecZ = distance.z / camera_moveZ_speed;
	moveVecZ = moveVecZ * 0.96f;
	return pos_.z + moveVecZ;

}