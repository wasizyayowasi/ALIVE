#include "Elevator.h"
#include "Switch.h"
#include "Lever.h"
#include "../util/ExternalFile.h"
#include "../util/Model.h"
#include "../util/Util.h"
#include "../util/InputState.h"

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
	int elevatorNum = StrUtil::GetNumberFromString(objInfo.name, ".");


	//エレベーターの番号と文字列を連結させた文字列を取得する
	std::string switchName = StrUtil::GetConcatenateNumAndStrings("ElevatorSwitch", ".", elevatorNum);

	//上記で連結した名前を持つデータを取得してインスタンス化する
	switch_ = std::make_shared<Switch>(ExternalFile::GetInstance().GetSpecifiedGimmickInfo(pos_, switchName.c_str()));

	//エレベーターの番号と文字列を連結させた文字列を取得する
	std::string pointName = StrUtil::GetConcatenateNumAndStrings("ElevatorPoint", ".", elevatorNum);

	//エレベーターの番号と文字列を連結させた文字列を取得する
	std::string leverName = StrUtil::GetConcatenateNumAndStrings("ElevatorLever", ".", elevatorNum);

	for (int i = 0; i < 2; i++) {
		VECTOR stopPoint = ExternalFile::GetInstance().GetSpecifiedGimmickInfo(pos_, pointName.c_str()).pos;
		levers_.push_back(std::make_shared<Lever>(ExternalFile::GetInstance().GetSpecifiedGimmickInfo(pos_, leverName.c_str()), stopPoint));
	}

	model_->SetAnimation(static_cast<int>(ElevatorAnimType::openIdle), true, false);

}

Elevator::~Elevator()
{
}

void Elevator::Update(Player& player, const InputState& input)
{
	VECTOR playerPos = player.GetStatus().pos;

	model_->Update();

	//スイッチの更新
	switch_->Update(player);

	for (auto lever : levers_) {
		lever->Update();
	}

	for (auto lever : levers_) {
		if (lever->CollCheck(playerPos)) {
			player.SetLeverPointer(lever);
		}
	}

	for (auto lever : levers_) {
		float size = MathUtil::GetSizeOfDistanceTwoPoints(targetPos_, lever->GetElevatorStopPoint());
		if (size != 0.0f) {
			if (lever->IsOn()) {
				targetPos_ = lever->GetElevatorStopPoint();
				model_->ChangeAnimation(static_cast<int>(ElevatorAnimType::close), false, false, 10);
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

	if (model_->IsAnimEnd()) {
		Move();
	}

	VECTOR playerTopPos = VGet(playerPos.x, playerPos.y + player.GetStatus().height, playerPos.z);
	auto result = MV1CollCheck_Capsule(model_->GetModelHandle(), model_->GetColFrameIndex(), playerPos, playerTopPos, 30.0f);

	if (result.HitNum > 0) {
		playerPos.y = result.Dim->Position->y;
		player.SetPos(playerPos);
	}

	MV1CollResultPolyDimTerminate(result);

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
	//移動
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
		model_->ChangeAnimation(static_cast<int>(ElevatorAnimType::open), false, false, 10);
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
				model_->ChangeAnimation(static_cast<int>(ElevatorAnimType::close), false, false, 10);
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
