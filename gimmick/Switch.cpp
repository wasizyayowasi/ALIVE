#include "Switch.h"
#include "../util/Model.h"
#include "../OBJECT/Player.h"

namespace {
	const VECTOR scale = { 0.5f,0.5f, 0.5f };
}

Switch::Switch(const char* const filename, LoadObjectInfo objInfo) :GimmickBase(filename,objInfo)
{
	model_->SetCollFrame();
	model_->SetAnimation(0, true, false);
}

Switch::Switch(int handle, LoadObjectInfo objInfo) :GimmickBase(handle,objInfo)
{
	model_->SetCollFrame();
	model_->SetAnimation(0, true, false);
}

Switch::~Switch()
{
}

//�X�V
void Switch::Update(Player& player)
{

	hit = false;

	//�A�j���[�V�����̍X�V
	model_->Update();

	//�Փ˔���
	HitCollPlayer(player);

	isTransit_ = player.GetStatus().isTransit;

	//�Փˌ��ʂ̍폜
	for (auto& result : hitDim_) {
		MV1CollResultPolyDimTerminate(result);
	}

	//�Փˌ��ʂ�ێ�����z��̍폜
	hitDim_.erase(hitDim_.begin(), hitDim_.end());

}

void Switch::Draw()
{
	model_->Draw();
	if (hit) {
		DrawSphere3D(VAdd(pos_, VGet(0, 200, 0)), 50, 21, 0x00ffff, 0x00ff00, true);
	}
}

//�Փ˔���
void Switch::HitCollPlayer(Player& player)
{
	//�v���C���[�̈ʒu�������ɂ����J�v�Z���ƃX�C�b�`���f���̔���
	hitDim_.push_back(MV1CollCheck_Capsule(model_->GetModelHandle(), model_->GetColFrameIndex(), VAdd(player.GetStatus().pos, VGet(0.0f, 10.0f, 0.0f)), VAdd(player.GetStatus().pos, VGet(0.0f, 150.0f, 0.0f)), 20.0f));

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

	//�Փ˔���̏���
	for (auto& hit : hitDim_) {
		MV1CollResultPolyDimTerminate(hit);
	}

	hitDim_.clear();

	hit = true;

	//�A�j���[�V�����̕ύX
	OnAnim();
}

void Switch::HitColl(std::shared_ptr<ObjectBase> deadPerson)
{

	//�����^�ђ���������ȍ~�̏������s��Ȃ�
	if (isTransit_) {
		return;
	}

	//�v���C���[�̈ʒu�������ɂ����J�v�Z���ƃX�C�b�`���f���̔���
	hitDim_.push_back(MV1CollCheck_Capsule(deadPerson->GetModelPointer()->GetModelHandle(), deadPerson->GetModelPointer()->GetColFrameIndex(), pos_, VAdd(pos_, VGet(0.0f, 50.0f, 0.0f)), 50.0f));

	int hitNum = 0;
	//�������Ă��鐔�𐔂���
	for (auto& result : hitDim_) {
		if (result.HitNum > 1) {
			hitNum++;
		}
	}

	//�������Ă��Ȃ�������
	//�A�j���[�V������ύX���I��
	if (hitNum < 1) {
		OffAnim();
		return;
	}

	//�Փ˔���̏���
	for (auto& hit : hitDim_) {
		MV1CollResultPolyDimTerminate(hit);
	}

	hitDim_.clear();

	hit = true;

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
