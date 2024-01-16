#include "Player1.h"
#include "../util/InputState.h"
#include "ObjectManager.h"

void Player1::NormalUpdate(const InputState& input, std::shared_ptr<ObjectManager> objManager)
{
	////�f�o�b�O�p�@�폜�\��
	//if (input.IsTriggered(InputType::creative)) {
	//	//debugCreativeMode = !debugCreativeMode;
	//}

	////�I�u�W�F�N�g�ɑ΂��ăA�N�V�������N����
	//if (input.IsTriggered(InputType::activate)) {
	//	status_.moveVec = VGet(0, 0, 0);
	//	if (status_.situation.isCanBeCarried) {
	//		(this->*carryUpdateFunc_)();
	//	}
	//	else if (status_.situation.isGimmickCanBeOperated) {
	//		if (crank_ != nullptr) {
	//			//�N�����N�𓮂�������������
	//			updateFunc_ = &Player::GoCrankRotationPosition;
	//		}
	//		else if (lever_ != nullptr) {
	//			//���o�[�𓮂�������������
	//			ChangeAnimNo(PlayerAnimType::LeverOn, false, 10);
	//			lever_->OnAnimation();
	//			updateFunc_ = &Player::GoLeverPullPosition;
	//		}
	//		return;
	//	}
	//	else {
	//		updateFunc_ = &Player::IdleToSitup;
	//		return;
	//	}
	//}
	//else {
	//	crank_.reset();
	//	lever_.reset();
	//}

	//status_.situation.isGimmickCanBeOperated = false;

	//if (status_.situation.isInTransit) {
	//	dynamic_pointer_cast<DeadPerson>(deadPersonModelPointer_)->Init();
	//	deadPersonModelPointer_->GetModelPointer()->SetRot(MathUtil::VECTORDegreeToRadian(status_.rot));
	//	VECTOR framePos = model_->GetFrameLocalPosition(frame_name);
	//	deadPersonModelPointer_->GetModelPointer()->SetPos(framePos);
	//}
	//else {
	//	status_.situation.isCanBeCarried = false;
	//}

	//ChangeAnimIdle();
	//MovingUpdate(input);

	////�󒆂ɂ���Ƃ�
	////�d�͂��x�N�g���ɑ����ă|�W�V�����ɑ���
	//if (!debugCreativeMode) {
	//	if (status_.jump.isJump) {
	//		status_.jump.jumpVec += gravity;
	//		status_.pos.y += status_.jump.jumpVec;
	//		model_->SetPos(status_.pos);
	//	}
	//	else {
	//		status_.jump.jumpVec = 0.0f;
	//	}
	//}


	////�����^�ђ���������
	////�ȍ~�̏������s��Ȃ�
	//if (status_.situation.isInTransit) {
	//	return;
	//}

	////�����o�֐��|�C���^��runningJumpUpdate�A
	////jumpUpdate�̂ǂ��炩�ɕύX����
	//if (!debugCreativeMode) {
	//	if (input.IsTriggered(InputType::space)) {
	//		if (!status_.jump.isJump) {
	//			PlayerJump(playerInfo_.jumpPower);
	//		}
	//		ChangeAnimNo(PlayerAnimType::jump, false, 20);
	//		updateFunc_ = &Player::JumpUpdate;
	//		return;
	//	}
	//}
	//else {
	//	if (input.IsPressed(InputType::space)) {
	//		status_.moveVec.y = 10.0;
	//	}
	//}

	////�����o�֐��|�C���^��DeathUpdate�ɕύX����
	//if (input.IsTriggered(InputType::death)) {
	//	deathCount_++;
	//	updateFunc_ = &Player::DeathUpdate;
	//	status_.moveVec = VGet(0, 0, 0);
	//	return;
	//}
}
