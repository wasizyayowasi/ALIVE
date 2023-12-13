#include "Elevator.h"
#include "Switch.h"
#include "Lever.h"
#include "../util/ExternalFile.h"
#include "../util/Model.h"
#include "../util/Util.h"
#include "../util/InputState.h"

#include "../object/Player.h"

namespace {
	//�G���x�[�^�[���ړ����鑬�x
	constexpr float move_speed = 10.0f;

	//�o�����鎞��
	constexpr int max_departure_time = 60;
}

//�R���X�g���N�^
Elevator::Elevator(int handle, LoadObjectInfo objInfo):GimmickBase(handle,objInfo)
{
	//�|�W�V�����̏�����
	pos_ = objInfo.pos;

	//�^�[�Q�b�g�|�W�V�����̏�����
	targetPos_ = pos_;

	//���̃G���x�[�^�[�����Ԗڂ̃G���x�[�^�[���𖼑O�̖�������擾����
	int elevatorNum = StrUtil::GetNumberFromString(objInfo.name, ".");

	//�G���x�[�^�[�̔ԍ��ƕ������A����������������擾����
	std::string switchName = StrUtil::GetConcatenateNumAndStrings("ElevatorSwitch", ".", elevatorNum);

	//��L�ŘA���������O�����f�[�^���擾���ăC���X�^���X������
	switch_ = std::make_shared<Switch>(ExternalFile::GetInstance().GetSpecifiedGimmickInfo(pos_, switchName.c_str()));

	//�G���x�[�^�[�̔ԍ��ƕ������A����������������擾����
	std::string pointName = StrUtil::GetConcatenateNumAndStrings("ElevatorPoint", ".", elevatorNum);

	//�G���x�[�^�[�̔ԍ��ƕ������A����������������擾����
	std::string leverName = StrUtil::GetConcatenateNumAndStrings("ElevatorLever", ".", elevatorNum);

	//���o�[�̃C���X�^���X��
	for (int i = 0; i < 2; i++) {
		VECTOR stopPoint = ExternalFile::GetInstance().GetSpecifiedGimmickInfo(pos_, pointName.c_str()).pos;
		levers_.push_back(std::make_shared<Lever>(ExternalFile::GetInstance().GetSpecifiedGimmickInfo(pos_, leverName.c_str()), stopPoint));
	}

	//�A�j���[�V�����̐ݒ�
	model_->SetAnimation(static_cast<int>(ElevatorAnimType::openIdle), true, false);

}

//�f�X�g���N�^
Elevator::~Elevator()
{
}

//�X�V
void Elevator::Update(Player& player, const InputState& input)
{
	//�v���C���[�̃|�W�V�����̎擾
	VECTOR playerPos = player.GetStatus().pos;

	//���f���̍X�V
	model_->Update();

	//�X�C�b�`�̍X�V
	switch_->Update(player);

	//���o�[�̍X�V
	for (auto lever : levers_) {
		lever->Update();
	}

	//���o�[�ƃv���C���[�̏Փ˔�����s��
	//�Փ˂��Ă����ꍇ�v���C���[�Ƀ|�C���^��ݒ肷��
	for (auto lever : levers_) {
		if (lever->CollCheck(playerPos)) {
			player.SetLeverPointer(lever);
		}
	}

	//���o�[�������ꂽ��A�j���[�V������ύX����
	//�ړI�n�܂ňړ�����
	for (auto lever : levers_) {
		float size = MathUtil::GetSizeOfDistanceTwoPoints(targetPos_, lever->GetElevatorStopPoint());
		if (size != 0.0f) {
			if (lever->IsOn()) {
				targetPos_ = lever->GetElevatorStopPoint();
				model_->ChangeAnimation(static_cast<int>(ElevatorAnimType::close), false, false, 10);
			}
		}
	}

	//�^�[�Q�b�g�|�W�V�����̕ύX
	if (targetPos_.y == pos_.y && !isDeparture_) {
		TargetPosition();
	}
	else {
		if (!switch_->CollResult()) {
			isDeparture_ = false;
		}
		switch_->DeleteHitResult();
	}

	//�A�j���[�V�������I������A�ړ����J�n����
	if (model_->IsAnimEnd()) {
		Move();
	}

//	VECTOR playerTopPos = VGet(playerPos.x, playerPos.y + player.GetStatus().height, playerPos.z);
//	auto result = MV1CollCheck_Capsule(model_->GetModelHandle(), model_->GetColFrameIndex(), playerPos, playerTopPos, 30.0f);
//
//	if (result.HitNum > 0) {
//		playerPos.y = result.Dim->Position->y;
//		player.SetPos(playerPos);
//	}
//
//	MV1CollResultPolyDimTerminate(result);

}

//�`��
void Elevator::Draw()
{
	//���f���̕`��
	model_->Draw();

	//�X�C�b�`�̕`��
	switch_->Draw();

	//���o�[�̕`��
	for (auto lever : levers_) {
		lever->Draw();
	}

//	DrawFormatString(0, 32, 0xff0000, "%.2f,%.2f,%.2f", pos_.x, pos_.y, pos_.z);
//	DrawFormatString(0, 48, 0xff0000, "%.2f,%.2f,%.2f", switch_[0]->GetPos().x, switch_[0]->GetPos().y, switch_[0]->GetPos().z);
//	DrawFormatString(0, 64, 0xff0000, "%.2f,%.2f,%.2f", switch_[1]->GetPos().x, switch_[1]->GetPos().y, switch_[1]->GetPos().z);

}

//�G���x�[�^�[���ړ�������
void Elevator::Move()
{
	//�ړ�
	float distance = targetPos_.y - pos_.y;

	//Y���̈ړ��x�N�g�����擾
	moveVecY_ = distance / 0.96f;

	//���̐��l�̈ړ��x�N�g�����擾
	moveVecY_ = (std::max)(moveVecY_, -moveVecY_);

	//�ړ��x�N�g����move_speed�����傫��������
	//�ړ��x�N�g����move_speed�ɂ���
	if (moveVecY_ > move_speed) {
		float scale = move_speed / distance;
		scale = (std::max)(scale, -scale);
		moveVecY_ = distance * scale;
	}

	//�^�[�Q�b�g�|�W�V�����ƃG���x�[�^�[�̃|�W�V������
	//���̋����ȓ���������^�[�Q�b�g�|�W�V�������|�W�V�����Ƃ���
	distance = (std::max)(distance, -distance);

	if (distance < 3.0f) {
		pos_ = targetPos_;
		model_->ChangeAnimation(static_cast<int>(ElevatorAnimType::open), false, false, 10);
	}
	else {
		pos_.y += moveVecY_;
	}

	//�|�W�V�����̐ݒ�
	model_->SetPos(pos_);
	//�X�C�b�`�̃|�W�V�����̐ݒ�
	switch_->GetModelPointer()->SetPos(VGet(pos_.x, pos_.y + 8.0f, pos_.z));
}

//�ړ���̃|�W�V�������擾����
void Elevator::TargetPosition()
{
	float distanceSize = {};
	float maxSize = 0.0f;

	//��ԉ������o�[���������Ă���|�W�V�������擾����
	if (switch_->CollResult()) {
		for (auto lever : levers_) {
			VECTOR stopPos = lever->GetElevatorStopPoint();
			distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(pos_, stopPos);
			if (maxSize < distanceSize) {
				maxSize = distanceSize;
				targetPos_ = stopPos;
				model_->ChangeAnimation(static_cast<int>(ElevatorAnimType::close), false, false, 10);
			}
		}
		isDeparture_ = true;
	}
}

//�Փ˔�����s�����f���̒ǉ�
std::shared_ptr<Model> Elevator::AddCollModel()
{
	return switch_->GetModelPointer();
}
