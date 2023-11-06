#include "ManualCrank.h"
#include "../util/Model.h"
#include "../util/InputState.h"
#include <algorithm>

namespace {
	const char* const box_filename = "data/model/box.mv1";

	const VECTOR center_point = VGet(-200, 100, 0);

	constexpr float top_angle = 45.0f;
	constexpr float right_angle = 135.0f;
	constexpr float bottom_angle = 225.0f;
	constexpr float left_angle = 315.0f;
}

ManualCrank::ManualCrank()
{
	model_ = std::make_shared<Model>(box_filename);
	model_->SetPos(pos_);
	model_->SetScale(VGet(50, 50, 50));
}

ManualCrank::~ManualCrank()
{
	
}

void ManualCrank::Update(const InputState& input)
{

	AngleUpdate(input);
	CurrentStateUpdate();

	float radion = rotZ_ * DX_PI_F / 180.0f;

	float x = sin(radion);
	float z = cos(radion);

	pos_.x = center_point.x + x * 40;
	pos_.y = center_point.y + z * 40;
	
//	rotZ_ += 1.0f;

	model_->SetPos(pos_);

	if (rotZ_ == 360.0f) {
		rotZ_ = 0.0f;
	}

}

void ManualCrank::Draw()
{
	model_->Draw();
//	DrawFormatString(0, 32, 0x448844, "%.2f,%.2f,%.2f", pos_.x, pos_.y, pos_.z);
//	DrawSphere3D(VGet(-200, 100, 0), 100, 32, 0xff0000, 0xff0000, true);

	VECTOR temp = ConvWorldPosToScreenPos(VGet(-200, 100, 0));
	DrawCircle(temp.x, temp.y, 100, 0xff0000,false);

	switch (currentState_) {
	case State::top:
		DrawString(temp.x, temp.y - 100, "top", 0x448844);
		break;
	case State::right:
		DrawString(temp.x + 100, temp.y, "right", 0x448844);
		break;
	case State::bottom:
		DrawString(temp.x, temp.y + 100, "bottom", 0x448844);
		break;
	case State::left:
		DrawString(temp.x - 100, temp.y, "left", 0x448844);
		break;
	}

}

void ManualCrank::AngleUpdate(const InputState& input)
{
	if (input.IsPressed(InputType::right)) {
		rotZ_ = (std::min)(rotZ_ + 1.0f, 90.0f);
	}
	if (input.IsPressed(InputType::down)) {
		rotZ_ = (std::min)(rotZ_ + 1.0f, 180.0f);
	}
	if (input.IsPressed(InputType::left)) {
		rotZ_ = (std::min)(rotZ_ + 1.0f, -90.0f);
	}
	if (input.IsPressed(InputType::up)) {
		rotZ_ = (std::min)(rotZ_ + 1.0f, 0.0f);
	}
}

void ManualCrank::CurrentStateUpdate()
{
	float innerProduct = VDot(VGet(0, 1, 0), VNorm(VSub(pos_, center_point)));
	float angle = acos(innerProduct);
	innerProduct = angle / DX_PI_F * 180.0f;

	if (innerProduct < top_angle && innerProduct < -top_angle) {
		currentState_ = State::top;
	}
	if (innerProduct >= top_angle && innerProduct < right_angle) {
		currentState_ = State::right;
	}
	if (innerProduct >= right_angle && innerProduct < bottom_angle) {
		currentState_ = State::bottom;
	}
	if (innerProduct >= bottom_angle && innerProduct < 360.0f - top_angle) {
		currentState_ = State::left;
	}


}
