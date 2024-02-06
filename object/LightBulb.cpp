#include "LightBulb.h"

#include "../util/Model.h"
#include "../util/Easing.h"

#include <algorithm>

namespace {
	//‘ŠÔ
	constexpr float total_time = 360.0f;
}

LightBulb::LightBulb(int handle, LoadObjectInfo info)
{
	//ƒ‚ƒfƒ‹‚Ìİ’è
	model_ = std::make_shared<Model>(handle,Material::Iron);
	model_->SetPos(info.pos);
	model_->SetRot(info.rot);
	model_->SetScale(info.scale);

	rot_ = info.rot;
}

LightBulb::~LightBulb()
{
}

void LightBulb::Update()
{
	elapsedTime_ = (std::min)(elapsedTime_ + 1,total_time);

	angle_ = Easing::InOutSine(elapsedTime_, total_time, targetAngle_, angle_);

	if (elapsedTime_ >= total_time / 4) {
		elapsedTime_ = 0.0f;
		targetAngle_ -= targetAngle_ * 2;
	}

	rot_.z = angle_ * DX_PI_F / 180.0f;

	model_->SetRot(rot_);
}

void LightBulb::Draw()
{
	model_->Draw();
}

VECTOR LightBulb::GetFrontVec()
{

	MATRIX mtxRotZ = MGetRotZ(rot_.z);

	VECTOR frontVec = VTransform({ 0,-1,0 }, mtxRotZ);

	return frontVec;
}

VECTOR LightBulb::GetFramePos()
{
	VECTOR pos = model_->GetFrameLocalPosition("LightBulb");
	return pos;
}
