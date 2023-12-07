#include "Elevator.h"
#include "Switch.h"
#include "../util/ExternalFile.h"
#include "../util/Model.h"
#include "../util/Util.h"

#include "../object/Player.h"

namespace {
	//エレベーターが移動する速度
	constexpr float move_speed = 10.0f;

	//出発する時間
	constexpr int max_departure_time = 60;
}

Elevator::Elevator(int handle, LoadObjectInfo objInfo):GimmickBase(handle,objInfo)
{
	pos_ = objInfo.pos;

	targetPos_ = pos_;

	//このエレベーターが何番目のエレベーターかを名前の末尾から取得する
	int switchNum = StrUtil::GetNumberFromString(objInfo.name, ".");
	//上記で取得した番号と文字列を連結させた文字列を取得する
	std::string switchName = StrUtil::GetConcatenateNumAndStrings("ElevatorSwitch", ".", switchNum);

	//上記で連結した名前を持つデータを取得してインスタンス化する
	switch_ = std::make_shared<Switch>(ExternalFile::GetInstance().GetSpecifiedGimmickInfo(pos_, switchName.c_str()));

	//このエレベーターが何番目のエレベーターかを名前の末尾から取得する
	int pointNum = StrUtil::GetNumberFromString(objInfo.name, ".");
	//上記で取得した番号と文字列を連結させた文字列を取得する
	std::string pointName = StrUtil::GetConcatenateNumAndStrings("ElevatorPoint", ".", pointNum);

	//エレベーターが移動するポジションを取得する
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
	VECTOR playerPos = player.GetStatus().pos;

	//スイッチの更新
	switch_->Update(player);

	//
	if (targetPos_.y == pos_.y && !isDeparture_) {
		PlayerTracking(playerPos);
		TargetPosition();
	}
	else {
		if (!switch_->CollResult()) {
			isDeparture_ = false;
		}
		switch_->DeleteHitResult();
	}

	//移動
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
