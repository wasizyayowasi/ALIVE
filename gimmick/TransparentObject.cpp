#include "TransparentObject.h"
#include "Switch.h"
#include "../util/Model.h"
#include "../util/ExternalFile.h"
#include "../util/Util.h"
#include <algorithm>

TransparentObject::TransparentObject(int handle, LoadObjectInfo objInfo) : GimmickBase(handle, objInfo)
{
	//�Փ˔���p�t���[���̐ݒ�
	model_->SetUseCollision(true, false);

	//������̖����𐔒l�Ƃ��Ď擾����
	int num = StrUtil::GetNumberFromString(objInfo.name, ".");
	//�������Ɛ��l��A����������������擾
	std::string name = StrUtil::GetConcatenateNumAndStrings("TransSwitch", ".", num);

	//�f�[�^�̎擾
	auto info = ExternalFile::GetInstance().GetSpecifiedGimmickInfo(objInfo.pos, name.c_str());
	//�X�C�b�`�̃C���X�^���X��
	switch_ = std::make_shared<Switch>(info);

	//�Փ˔�����s��Ȃ���Ԃŏ�����
	isCollCheck_ = false;

	//���f���̃}�e���A���̃A���t�@�l��0�ɂ��ē��߂�����
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

void TransparentObject::Update(Player& player)
{
	switch_->Update(player);

	//�X�C�b�`���N�����Ă�����
	//���f���Ƃ̏Փ˔�����s���悤�ɂ���
	if (switch_->TransCollResult()) {
		isCollCheck_ = true;
	}
	else {
		isCollCheck_ = false;
	}

	//�A���t�@�l��ύX����
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

void TransparentObject::UpdateForCorpse(std::shared_ptr<ObjectBase> deadPerson)
{
	switch_->HitColl(deadPerson);
}

std::shared_ptr<Model> TransparentObject::AddCollModel()
{
	return switch_->GetModelPointer();
}
