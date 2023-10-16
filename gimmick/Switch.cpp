#include "Switch.h"
#include "../util/Model.h"
#include "../OBJECT/Player.h"

namespace {
	const VECTOR scale = { 0.5f,0.5f, 0.5f };
}

Switch::Switch(const char* const filename, LoadObjectInfo objInfo) :GimmickBase(filename,objInfo)
{
	model_->SetScale(scale);
	model_->SetPos({ 2245,0,0 });
	model_->SetCollFrame();
	model_->SetAnimation(0, true, false);
}

Switch::Switch(int handle, LoadObjectInfo objInfo) :GimmickBase(handle,objInfo)
{
	model_->SetScale(scale);
	model_->SetPos({ 2245,0,0 });
	model_->SetCollFrame();
	model_->SetAnimation(0, true, false);
}

Switch::~Switch()
{
}

//�X�V
void Switch::Update(Player& player)
{
	//�A�j���[�V�����̍X�V
	model_->Update();

	//�Փ˔���
	HitColl(player);

	//�Փˌ��ʂ̍폜
	for (auto& result : hitDim_) {
		MV1CollResultPolyDimTerminate(result);
	}

	//�Փˌ��ʂ�ێ�����z��̍폜
	hitDim_.erase(hitDim_.begin(), hitDim_.end());

}

//�Փ˔���
void Switch::HitColl(Player& player)
{
	//�v���C���[�̈ʒu�������ɂ����J�v�Z���ƃX�C�b�`���f���̔���
	hitDim_.push_back(MV1CollCheck_Capsule(model_->GetModelHandle(), model_->GetColFrameIndex(), VAdd(player.GetStatus().pos, VGet(0.0f, 10.0f, 0.0f)), VAdd(player.GetStatus().pos, VGet(0.0f, 150.0f, 0.0f)), 20.0f));

	//���̂̈ʒu�������ɂ����J�v�Z���ƃX�C�b�`���f���̔���
	/*for (auto& person : player.getDeadPerson()) {
		hitDim_.push_back(MV1CollCheck_Capsule(model_->getModelHandle(), model_->getColFrameIndex(), VAdd(person->getPos(), VGet(0.0f, 0.0f, 0.0f)), person->getPos(), 20.0f));
	}*/

	int hitNum = 0;
	//�������Ă��鐔�𐔂���
	for (auto& result : hitDim_) {
		if (result.HitNum < 1) continue;
		hitNum++;
	}

	//�������Ă��Ȃ�������
	//�A�j���[�V������ύX���I��
	if (hitNum < 1) {
		OffAnim();
		return;
	}

	//�A�j���[�V�����̕ύX
	OnAnim();
}

//�X�C�b�`�I���A�j���[�V����
void Switch::OnAnim()
{
	model_->SetAnimation(1, false, false);
}

//�X�C�b�`�I�t�A�j���[�V����
void Switch::OffAnim()
{
	model_->SetAnimation(2, false, false);
}
