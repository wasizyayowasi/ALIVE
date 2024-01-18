#include "PenetrationScaffld.h"

#include "../util/Model.h"

#include "../object/Player.h"

PenetrationScaffld::PenetrationScaffld(int handle, LoadObjectInfo objInfo):GimmickBase(handle,objInfo)
{
	//���f���̃}�e���A���̃A���t�@�l��0�ɂ��ē��߂�����
	materialNum_ = MV1GetMaterialNum(model_->GetModelHandle());

	//�����蔻����s��Ȃ�
	isCollCheck_ = false;
}

PenetrationScaffld::~PenetrationScaffld()
{
}

void PenetrationScaffld::Update(Player& player)
{
	//�J���[�ϐ�
	COLOR_F color = {};

	//�A���t�@�l�𑝌�����
	if (alphaValueIncrease_) {
		alphaValue_ = (std::min)(alphaValue_ + 0.001f, 1.0f);
		if (alphaValue_ == 1.0f) {
			alphaValueIncrease_ = false;
		}
	}
	else {
		alphaValue_ = (std::max)(alphaValue_ - 0.001f, 0.8f);
		if (alphaValue_ == 0.8f) {
			alphaValueIncrease_ = true;
		}
	}

	//�}�e���A���̃A���t�@�l��ύX����
	for (int i = 0; i < materialNum_; i++) {
		color = MV1GetMaterialDifColor(model_->GetModelHandle(), i);
		color.a = alphaValue_;
		MV1SetMaterialDifColor(model_->GetModelHandle(), i, color);
	}
}

void PenetrationScaffld::Draw()
{
	model_->Draw();
}
