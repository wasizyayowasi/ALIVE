#include "Elevator.h"
#include "Switch.h"
#include "../util/ExternalFile.h"
#include "../util/Model.h"

namespace {
	constexpr float move_speed = 10.0f;
}

Elevator::Elevator(const char* const filename, LoadObjectInfo objInfo):GimmickBase(filename,objInfo)
{
	model_ = std::make_shared<Model>(filename);

	pos_ = objInfo.pos;

	for (int i = 0; i < 2; i++) {
		switch_.push_back(std::make_shared<Switch>(ExternalFile::GetInstance().GetSpecifiedGimmickInfo(pos_,"Switch")));
	}

}

Elevator::Elevator(int handle, LoadObjectInfo objInfo):GimmickBase(handle,objInfo)
{
	pos_ = objInfo.pos;

	targetPos = pos_;

	for (int i = 0; i < 2; i++) {
		switch_.push_back(std::make_shared<Switch>(ExternalFile::GetInstance().GetSpecifiedGimmickInfo(pos_, "Switch")));
	}

}

Elevator::~Elevator()
{
}

void Elevator::Update(Player& player, const InputState& input)
{
	float distance = 0.0f;

	for (auto& bottan : switch_) {
		bottan->Update(player);
	}

	if (targetPos.y == pos_.y) {
		TargetPosition(distance);
	}
	else {
		for (auto& bottan : switch_) {
			bottan->DeleteHitResult();
		}
	}

	//ˆÚ“®
	distance = targetPos.y - pos_.y;

	moveVecY = distance / 0.96f;

	moveVecY = (std::max)(moveVecY, -moveVecY);

	if (moveVecY > move_speed) {
		float scale = move_speed / distance;
		scale = (std::max)(scale, -scale);
		moveVecY = distance * scale;
	}

	distance = (std::max)(distance, -distance);

	if (distance < 3.0f) {
		pos_ = targetPos;
	}
	else {
		pos_.y += moveVecY;
	}

	model_->SetPos(pos_);

}

void Elevator::Draw()
{
	model_->Draw();

	for (auto& bottan : switch_) {
		bottan->Draw();
	}

//	DrawFormatString(0, 32, 0xff0000, "%.2f,%.2f,%.2f", pos_.x, pos_.y, pos_.z);
//	DrawFormatString(0, 48, 0xff0000, "%.2f,%.2f,%.2f", switch_[0]->GetPos().x, switch_[0]->GetPos().y, switch_[0]->GetPos().z);
//	DrawFormatString(0, 64, 0xff0000, "%.2f,%.2f,%.2f", switch_[1]->GetPos().x, switch_[1]->GetPos().y, switch_[1]->GetPos().z);

}

void Elevator::TargetPosition(float distanceY)
{
 	if (switch_[0]->CollResult()) {
		distanceY = switch_[0]->GetPos().y - pos_.y;
		distanceY = (std::max)(distanceY, -distanceY);
		if (distanceY < 5.0f) {
			targetPos = switch_[1]->GetPos();
		}
		else {
			targetPos = switch_[0]->GetPos();
		}
	}

	if (switch_[1]->CollResult()) {
		distanceY = switch_[1]->GetPos().y - pos_.y;
		distanceY = (std::max)(distanceY, -distanceY);
		if (distanceY < 5.0f) {
			targetPos = switch_[0]->GetPos();
		}
		else {
			targetPos = switch_[1]->GetPos();
		}
	}

	targetPos.z = pos_.z;

}
