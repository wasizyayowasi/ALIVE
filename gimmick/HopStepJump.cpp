#include "HopStepJump.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../util/Easing.h"

#include <string>
#include <stdio.h>
#include <algorithm>

namespace {
	constexpr float total_time = 180.0f;
}

HopStepJump::HopStepJump(int handle, Material materialType, LoadObjectInfo objInfo):GimmickBase(handle,materialType,objInfo)
{
	//�}�e���A���̐��̎擾
	materialNum_ = MV1GetMaterialNum(handle);

	//�����蔻��̐ݒ�
	model_->SetUseCollision(true, true);

	//�����蔻��̐ݒ�
	isCollCheck_ = true;

	//���Ԃ̐ݒ�
	elapseddTime_ = total_time;

	currentNum_ = StrUtil::GetNumberFromString(objInfo.name, ".");
}

HopStepJump::~HopStepJump()
{
}

void HopStepJump::Update(Player& player)
{
	//�^�C�}�[
	elapseddTime_ = (std::max)(elapseddTime_ - 1, 0.0f);

	//���݂̔ԍ���ύX����
	if (elapseddTime_ == 0.0f) {
		currentNum_ = (std::min)(currentNum_ + 1, materialNum_);
		elapseddTime_ = total_time;
	}

	//�}�e���A���̐��𒴂�����0�ɂ���
	if (currentNum_ == materialNum_) {
		currentNum_ = 0;
	}

	//�A���t�@�l��ύX����
	if (elapseddTime_ >= 120) {
		alphaValue_ = (std::min)(alphaValue_ + (1.0f / 60.0f), 1.0f);
	}
	else if(elapseddTime_ <= 60){
		alphaValue_ = (std::max)(alphaValue_ - (1.0f / 60.0f), 0.0f);
	}

	COLOR_F color = {};

	//���݂̔ԍ��ȊO�̃}�e���A���̃J���[�̃A���t�@�l��0�ɂ���
	for (int i = 0; i < materialNum_; i++) {
		color = MV1GetMaterialDifColor(model_->GetModelHandle(), i);

		if (currentNum_ == i) {
			color.a = alphaValue_;
		}
		else{
			color.a = 0.0f;
		}
		
		MV1SetMaterialDifColor(model_->GetModelHandle(), i, color);
	}

	std::string num = std::to_string(currentNum_);

	//�����蔻��p�t���[���̕ύX
	model_->SetCollFrame(num.c_str());

}

void HopStepJump::Draw()
{
	//model_->Draw();

	MV1DrawFrame(model_->GetModelHandle(), model_->GetColFrameIndex());
}