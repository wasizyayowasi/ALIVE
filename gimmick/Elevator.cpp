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

Elevator::Elevator(int handle, LoadObjectInfo objInfo):GimmickBase(handle,objInfo)
{
	pos_ = objInfo.pos;

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

	for (int i = 0; i < 2; i++) {
		VECTOR stopPoint = ExternalFile::GetInstance().GetSpecifiedGimmickInfo(pos_, pointName.c_str()).pos;
		levers_.push_back(std::make_shared<Lever>(ExternalFile::GetInstance().GetSpecifiedGimmickInfo(pos_, leverName.c_str()), stopPoint));
	}
}

Elevator::~Elevator()
{
}

void Elevator::Update(Player& player, const InputState& input)
{
	VECTOR playerPos = player.GetStatus().pos;

	//�X�C�b�`�̍X�V
	switch_->Update(player);

	for (auto lever : levers_) {
		lever->Update();
	}

	for (auto lever : levers_) {
		if (input.IsTriggered(InputType::activate)) {
			if (lever->CollCheck(playerPos)) {
				targetPos_ = lever->GetElevatorStopPoint();
			}
		}
	}

	//
	if (targetPos_.y == pos_.y && !isDeparture_) {
		TargetPosition();
	}
	else {
		if (!switch_->CollResult()) {
			isDeparture_ = false;
		}
		switch_->DeleteHitResult();
	}

	Move();

}

void Elevator::Draw()
{
	model_->Draw();

	switch_->Draw();

	for (auto lever : levers_) {
		lever->Draw();
	}

//	DrawFormatString(0, 32, 0xff0000, "%.2f,%.2f,%.2f", pos_.x, pos_.y, pos_.z);
//	DrawFormatString(0, 48, 0xff0000, "%.2f,%.2f,%.2f", switch_[0]->GetPos().x, switch_[0]->GetPos().y, switch_[0]->GetPos().z);
//	DrawFormatString(0, 64, 0xff0000, "%.2f,%.2f,%.2f", switch_[1]->GetPos().x, switch_[1]->GetPos().y, switch_[1]->GetPos().z);

}

void Elevator::Move()
{
	//�ړ�
	float distance = targetPos_.y - pos_.y;

	moveVecY_ = distance / 0.96f;

	moveVecY_ = (std::max)(moveVecY_, -moveVecY_);

	if (moveVecY_ > move_speed) {
		float scale = move_speed / distance;
		scale = (std::max)(scale, -scale);
		moveVecY_ = distance * scale;
	}

	distance = (std::max)(distance, -distance);

	if (distance < 3.0f) {
		pos_ = targetPos_;
	}
	else {
		pos_.y += moveVecY_;
	}

	model_->SetPos(pos_);

	switch_->GetModelPointer()->SetPos(VGet(pos_.x, pos_.y + 8.0f, pos_.z));
}

void Elevator::TargetPosition()
{
	float distanceSize = {};
	float maxSize = 0.0f;

	if (switch_->CollResult()) {
		for (auto lever : levers_) {
			VECTOR stopPos = lever->GetElevatorStopPoint();
			distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(pos_, stopPos);
			if (maxSize < distanceSize) {
				maxSize = distanceSize;
				targetPos_ = stopPos;
			}
		}
		isDeparture_ = true;
	}

	targetPos_.z = pos_.z;
}

std::shared_ptr<Model> Elevator::AddCollModel()
{
	return switch_->GetModelPointer();
}
