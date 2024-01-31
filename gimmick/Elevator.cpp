#include "Elevator.h"
#include "Switch.h"
#include "Lever.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/Easing.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"

#include "../object/Player.h"

namespace {
	//�G���x�[�^�[���ړ����鑬�x
	constexpr float move_speed = 6.0f;

	//�o�����鎞��
	constexpr int max_departure_time = 60;

	//���B����
	constexpr float total_time = 180.0f;
}

//�R���X�g���N�^
Elevator::Elevator(int handle, LoadObjectInfo objInfo):GimmickBase(handle,objInfo)
{
	//�|�W�V�����̏�����
	pos_ = objInfo.pos;

	//�^�[�Q�b�g�|�W�V�����̏�����
	targetPos_ = pos_;

	//�Փ˔���p�t���[���̐ݒ�
	model_->SetUseCollision(true, false);

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
		std::string specificPointName = StrUtil::GetConcatenateNumAndStrings(pointName, "-", i);
		std::string specificLeverName = StrUtil::GetConcatenateNumAndStrings(leverName, "-", i);
		VECTOR stopPoint = ExternalFile::GetInstance().GetSpecifiedGimmickInfo(pos_, specificPointName.c_str()).pos;
		levers_.push_back(std::make_shared<Lever>(ExternalFile::GetInstance().GetSpecifiedGimmickInfo(pos_, specificLeverName.c_str()), stopPoint));
	}

	//�A�j���[�V�����̐ݒ�
	model_->SetAnimation(static_cast<int>(ElevatorAnimType::openIdle), true, false);
}

//�f�X�g���N�^
Elevator::~Elevator()
{
}

//�X�V
void Elevator::Update(Player& player)
{
	//�v���C���[�̃|�W�V�����̎擾
	VECTOR playerPos = player.GetStatus().pos;

	//���f���̍X�V
	model_->Update();

	//�X�C�b�`�̍X�V
	switch_->ChangeDuringStartup(elapsedTime_);
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

	//�^�[�Q�b�g�|�W�V�����̕ύX
	if (targetPos_.y == pos_.y && !isDeparture_) {
		TargetPosition();
	}
	else {
		if (!switch_->ElevatorCollResult()) {
			isDeparture_ = false;
		}
		switch_->DeleteHitResult();
	}

	//�A�j���[�V�������I������A�ړ����J�n����
	if (model_->IsAnimEnd()) {
		Move();
	}
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
}

//�G���x�[�^�[���ړ�������
void Elevator::Move()
{
	//���Ԃ̍X�V
	elapsedTime_ = (std::min)(elapsedTime_ + 1.0f, total_time);

	//���W�̈ړ�
	pos_.y = Easing::InOutCubic(elapsedTime_, total_time, targetPos_.y, pos_.y);

	//�ړ��I����A�j���[�V������ύX����
	if (elapsedTime_ == 180.0f) {
		SoundManager::GetInstance().Play("door");
		model_->ChangeAnimation(static_cast<int>(ElevatorAnimType::open), false, false, 10);
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
	if (switch_->ElevatorCollResult()) {
		for (auto lever : levers_) {
			VECTOR stopPos = lever->GetElevatorStopPoint();
			distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(pos_, stopPos);
			if (maxSize < distanceSize) {
				maxSize = distanceSize;
				targetPos_ = stopPos;
				SoundManager::GetInstance().Play("door");
				model_->ChangeAnimation(static_cast<int>(ElevatorAnimType::close), false, false, 10);
				elapsedTime_ = 0;
			}
		}
		isDeparture_ = true;
	}

	//���o�[�������ꂽ��A�j���[�V������ύX����
	//�ړI�n�܂ňړ�����
	if (levers_.front()->IsOn()) {
		float size = MathUtil::GetSizeOfDistanceTwoPoints(targetPos_, levers_.front()->GetElevatorStopPoint());
		if (size != 0.0f) {
			targetPos_ = levers_.front()->GetElevatorStopPoint();
		}
		else {
			targetPos_ = levers_.back()->GetElevatorStopPoint();
		}
		SoundManager::GetInstance().Play("door");
		model_->ChangeAnimation(static_cast<int>(ElevatorAnimType::close), false, false, 10);
		elapsedTime_ = 0;
	}

	//���o�[�������ꂽ��A�j���[�V������ύX����
	//�ړI�n�܂ňړ�����
	if (levers_.back()->IsOn()) {
		float size = MathUtil::GetSizeOfDistanceTwoPoints(targetPos_, levers_.back()->GetElevatorStopPoint());
		if (size != 0.0f) {
			targetPos_ = levers_.back()->GetElevatorStopPoint();
		}
		else {
			targetPos_ = levers_.front()->GetElevatorStopPoint();
		}
		SoundManager::GetInstance().Play("door");
		model_->ChangeAnimation(static_cast<int>(ElevatorAnimType::close), false, false, 10);
		elapsedTime_ = 0;
	}
}

//�Փ˔�����s�����f���̒ǉ�
std::shared_ptr<Model> Elevator::AddCollModel()
{
	return switch_->GetModelPointer();
}
