#include "Lever.h"
#include "../util/Model.h"

namespace {
	const char* const filename = "data/model/other/mv1/Lever.mv1";
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

	if (model_->IsAnimEnd()) {
		OffAnimation();
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

void Lever::OnAnimation()
{
	model_->ChangeAnimation(static_cast<int>(AnimType::on), false, false, 10);
	isOn_ = true;
}

void Lever::OffAnimation()
{
	model_->ChangeAnimation(static_cast<int>(AnimType::off), false, false, 10);
	isOn_ = false;
}