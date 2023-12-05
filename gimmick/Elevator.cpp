#include "Elevator.h"
#include "Switch.h"
#include "../util/ExternalFile.h"
#include "../util/Model.h"
#include "../util/Util.h"

#include "../object/Player.h"

namespace {
	constexpr float move_speed = 10.0f;
}

Elevator::Elevator(int handle, LoadObjectInfo objInfo):GimmickBase(handle,objInfo)
{
	pos_ = objInfo.pos;

	targetPos_ = pos_;

	int switchNum = StrUtil::GetNumberFromString(objInfo.name, ".");
	std::string switchName = StrUtil::GetConcatenateNumAndStrings("ElevatorSwitch", ".", switchNum);

	switch_ = std::make_shared<Switch>(ExternalFile::GetInstance().GetSpecifiedGimmickInfo(pos_, switchName.c_str()));

	int pointNum = StrUtil::GetNumberFromString(objInfo.name, ".");
	std::string pointName = StrUtil::GetConcatenateNumAndStrings("ElevatorPoint", ".", pointNum);

	for (int i = 0; i < 2; i++) {
		destinationPos_.push_back(ExternalFile::GetInstance().GetSpecifiedGimmickInfo(pos_, pointName.c_str()).pos);
	}

}

Elevator::~Elevator()
{
}

void Elevator::Update(Player& player, const InputState& input)
{
	float distance = 0.0f;

	switch_->Update(player);

	if (targetPos_.y == pos_.y) {
		PlayerTracking(player.GetStatus().pos);
		TargetPosition();
	}
	else {
		switch_->DeleteHitResult();
	}

	//ˆÚ“®
	distance = targetPos_.y - pos_.y;

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

void Elevator::Draw()
{
	model_->Draw();

	switch_->Draw();

//	DrawFormatString(0, 32, 0xff0000, "%.2f,%.2f,%.2f", pos_.x, pos_.y, pos_.z);
//	DrawFormatString(0, 48, 0xff0000, "%.2f,%.2f,%.2f", switch_[0]->GetPos().x, switch_[0]->GetPos().y, switch_[0]->GetPos().z);
//	DrawFormatString(0, 64, 0xff0000, "%.2f,%.2f,%.2f", switch_[1]->GetPos().x, switch_[1]->GetPos().y, switch_[1]->GetPos().z);

}

void Elevator::PlayerTracking(VECTOR playerPos)
{

	float min = 10000.0f;

	for (auto pos : destinationPos_) {
		float size = MathUtil::GetSizeOfDistanceTwoPoints(playerPos, pos);
		if (min > size) {
			min = size;
			targetPos_ = pos;
		}
	}
}

void Elevator::TargetPosition()
{
	float distanceSize = {};
	float maxSize = 0.0f;

	if (switch_->CollResult()) {
		for (auto pos : destinationPos_) {
			distanceSize = MathUtil::GetSizeOfDistanceTwoPoints(pos_, pos);
			if (maxSize < distanceSize) {
				maxSize = distanceSize;
				targetPos_ = pos;
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
