#include "Lever.h"
#include "../util/Model.h"

namespace {
	const char* const filename = "data/model/other/mv1/Lever.mv1";

	constexpr float max_rot_z = 40.0f;
}

Lever::Lever(LoadObjectInfo info, VECTOR stopPos)
{
	model_ = std::make_shared<Model>(filename);

	pos_ = info.pos;

	elevatorStopPosition_ = stopPos;

	model_->SetPos(pos_);
	model_->SetScale(info.scale);
	model_->SetRot(info.rot);

}

Lever::~Lever()
{
}

void Lever::Update()
{
	model_->Update();

 	if (!isOperated_) {
		if (rotZ_ != -max_rot_z) {
			rotZ_ = (std::max)(rotZ_ - 3.0f, -max_rot_z);
			UndoRotation(rotZ_);
		}
	}
}

void Lever::Draw()
{
	model_->Draw();
}

bool Lever::CollCheck(VECTOR playerPos)
{
	auto result = MV1CollCheck_Sphere(model_->GetModelHandle(), model_->GetColFrameIndex(), playerPos, 50.0f);

	if (result.HitNum > 0) {
		MV1CollResultPolyDimTerminate(result);
		return true;
	}

	return false;
}

void Lever::UndoRotation(float rotZ)
{
	float radian = rotZ * DX_PI_F / 180.0f;

	int frameNo = MV1SearchFrame(model_->GetModelHandle(), "Lever");

	VECTOR pos = MV1GetFramePosition(model_->GetModelHandle(), frameNo);

	MATRIX mat = {};

	mat = MGetRotX(radian);

	MV1SetFrameUserLocalMatrix(model_->GetModelHandle(), frameNo, mat);
}

float Lever::GetMaxRotZ()
{
	return max_rot_z;
}

void Lever::SetOperate(bool isOperate)
{
	isOperated_ = isOperate;
}
