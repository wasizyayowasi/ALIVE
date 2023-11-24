#include "Elevator.h"
#include "Switch.h"
#include "../util/ExternalFile.h"
#include "../util/Model.h"

Elevator::Elevator(const char* const filename, LoadObjectInfo objInfo):GimmickBase(filename,objInfo)
{
	model_ = std::make_shared<Model>(filename);

	pos_ = objInfo.pos;

	for (int i = 0; i < 2; i++) {
		switch_.push_back(std::make_shared<Switch>(ExternalFile::GetInstance().GetSpecifiedGimmickInfo("Switch")));
	}

}

Elevator::Elevator(int handle, LoadObjectInfo objInfo):GimmickBase(handle,objInfo)
{
	pos_ = objInfo.pos;

	targetPos = pos_;

	for (int i = 0; i < 2; i++) {
		switch_.push_back(std::make_shared<Switch>(ExternalFile::GetInstance().GetSpecifiedGimmickInfo("Switch")));
	}

}

Elevator::~Elevator()
{
}

void Elevator::Update(Player& player, const InputState& input)
{
	VECTOR distance = {};
	float distanceSize = 0.0f;

	for (auto& bottan : switch_) {
		bottan->Update(player);
	}

	int temp = static_cast<int>(targetPos.y);
	int temp1 = static_cast<int>(pos_.y);

	if (temp == temp1) {
		TargetPosition(distance, distanceSize);
	}

	//ˆÚ“®
	distance = VSub(targetPos, pos_);
	distance.x = 0.0f;
	distance.z = 0.0f;
	moveVec= VScale(VScale(distance,0.01f), 1.0f);
	distanceSize = VSize(distance);

	if (distanceSize < 1.0f) {
		pos_ = targetPos;
	}
	else {
		pos_ = VAdd(pos_, moveVec);
	}

	model_->SetPos(pos_);

}

void Elevator::Draw()
{
	model_->Draw();

	for (auto& bottan : switch_) {
		bottan->Draw();
	}

	DrawFormatString(0, 32, 0xff0000, "%.2f,%.2f,%.2f", pos_.x, pos_.y, pos_.z);
	DrawFormatString(0, 48, 0xff0000, "%.2f,%.2f,%.2f", switch_[0]->GetPos().x, switch_[0]->GetPos().y, switch_[0]->GetPos().z);
	DrawFormatString(0, 64, 0xff0000, "%.2f,%.2f,%.2f", switch_[1]->GetPos().x, switch_[1]->GetPos().y, switch_[1]->GetPos().z);

}

void Elevator::TargetPosition(VECTOR& distance, float distanceSize)
{
 	if (switch_[0]->CollResult()) {
		distance = VSub(switch_[0]->GetPos(), pos_);
		distance.x = 0.0f;
		distance.z = 0.0f;
		distanceSize = VSize(distance);
		if (distanceSize < 5.0f) {
			targetPos = switch_[1]->GetPos();
		}
		else {
			targetPos = switch_[0]->GetPos();
		}
	}

	if (switch_[1]->CollResult()) {
		distance = VSub(switch_[1]->GetPos(), pos_);
		distance.x = 0.0f;
		distance.z = 0.0f;
		distanceSize = VSize(distance);
		if (distanceSize < 5.0f) {
			targetPos = switch_[0]->GetPos();
		}
		else {
			targetPos = switch_[1]->GetPos();
		}
	}

	targetPos.x = pos_.x;
	targetPos.z = pos_.z;

}
