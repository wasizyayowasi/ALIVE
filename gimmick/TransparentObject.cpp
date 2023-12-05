#include "TransparentObject.h"
#include "Switch.h"
#include "../util/Model.h"
#include "../util/ExternalFile.h"
#include "../util/Util.h"
#include <algorithm>

TransparentObject::TransparentObject(int handle, LoadObjectInfo objInfo) : GimmickBase(handle, objInfo)
{
	
	int num = StrUtil::GetNumberFromString(objInfo.name, ".");
	std::string name = StrUtil::GetConcatenateNumAndStrings("TransSwitch", ".", num);

	auto info = ExternalFile::GetInstance().GetSpecifiedGimmickInfo(objInfo.pos, name.c_str());
	switch_ = std::make_shared<Switch>(info);
	
	InitialPosition_ = model_->GetPos();

	isCollCheck_ = false;

	materialNum_ = MV1GetMaterialNum(model_->GetModelHandle());

	COLOR_F color = {};

	for (int i = 0; i < materialNum_;i++) {
		color = MV1GetMaterialDifColor(model_->GetModelHandle(), i);
		color.a = 0.0f;
		MV1SetMaterialDifColor(model_->GetModelHandle(), i, color);
	}

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


	COLOR_F color = {};

	for (int i = 0; i < materialNum_; i++) {
		color = MV1GetMaterialDifColor(model_->GetModelHandle(), i);
		color.a = alphaValue_;
		MV1SetMaterialDifColor(model_->GetModelHandle(), i, color);
	}

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

std::shared_ptr<Model> TransparentObject::AddCollModel()
{
	return switch_->GetModelPointer();
}
