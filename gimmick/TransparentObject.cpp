#include "TransparentObject.h"
#include "Switch.h"
#include "../util/Model.h"
#include "../util/LoadExternalFile.h"
#include <algorithm>

namespace {
	const char* const switch_filename = "data/model/switch.mv1";
}

TransparentObject::TransparentObject(const char* const filename, LoadObjectInfo objInfo):GimmickBase(filename,objInfo)
{
}

TransparentObject::TransparentObject(int handle, LoadObjectInfo objInfo) : GimmickBase(handle, objInfo)
{
	auto info = LoadExternalFile::GetInstance().GetGimmickInfo("switch");
	switch_ = std::make_shared<Switch>(switch_filename,info);
	
	InitialPosition_ = model_->GetPos();

	isCollCheck_ = false;

}

TransparentObject::~TransparentObject()
{
}

void TransparentObject::Update(Player& player, const InputState& input)
{
	switch_->Update(player);

	if (switch_->CollResult()) {
		isCollCheck_ = true;
	}
	else {
		isCollCheck_ = false;
	}

	if (isCollCheck_) {
		alphaValue_ = (std::min)(alphaValue_ + 0.1f, 1.0f);
	}
	else {
		alphaValue_ = (std::max)(alphaValue_ - 0.1f, 0.0f);
	}

	MV1SetMaterialDifColor(model_->GetModelHandle(), 0, GetColorF(0.0f, 0.0f, 1.0f, alphaValue_));

}

void TransparentObject::Draw()
{
	model_->Draw();

	switch_->Draw();
}

void TransparentObject::HitColl(std::shared_ptr<ObjectBase> deadPerson)
{
	switch_->HitColl(deadPerson);
}
