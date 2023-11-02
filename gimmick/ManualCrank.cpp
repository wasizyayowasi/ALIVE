#include "ManualCrank.h"
#include "../util/Model.h"

namespace {
	const char* const box_filename = "data/model/box.mv1";
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

void ManualCrank::Update()
{

	float temp = rotationZ_ * DX_PI_F / 180.0f;

	float x = cos(temp) * 1.0f;
	float z = sin(temp) * 1.0f;

	pos_.x = pos_.x + x;
	pos_.y = pos_.y + z;

	rotationZ_ += 1.00f;

	model_->SetPos(pos_);

	//MATRIX rotMtx = MGetRotZ(rotationZ_);

	//MV1SetFrameUserLocalMatrix(model_->GetModelHandle(), 0, rotMtx);

}

void ManualCrank::Draw()
{
	model_->Draw();
	DrawFormatString(0, 32, 0x448844, "%.2f,%.2f,%.2f", pos_.x, pos_.y, pos_.z);
}
