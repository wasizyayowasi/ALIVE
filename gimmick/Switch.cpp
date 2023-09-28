#include "Switch.h"
#include "../util/Model.h"
#include "../OBJECT/Player.h"

namespace {
	const VECTOR scale = { 0.5f,0.5f, 0.5f };
}

Switch::Switch() :GimmickBase("data/model/switch.mv1")
{
	model_->setScale(scale);
	model_->setPos({ 2245,0,0 });
	model_->setCollFrame();
	model_->setAnimation(0, true, false);
}

Switch::~Switch()
{
}

//�X�V
void Switch::update(Player& player)
{
	//�A�j���[�V�����̍X�V
	model_->update();

	//�Փ˔���
	hitColl(player);

	//�Փˌ��ʂ̍폜
	for (auto& result : hitDim_) {
		MV1CollResultPolyDimTerminate(result);
	}

	//�Փˌ��ʂ�ێ�����z��̍폜
	hitDim_.erase(hitDim_.begin(), hitDim_.end());

}

VECTOR Switch::deadPersonCapsulePosAdjustment(std::shared_ptr<Model> person)
{
	VECTOR rot = person->getRot();
	rot = VNorm(rot);
	rot = VScale(VNorm(rot),-100.0f);
	rot.y = 20.0f;

	return rot;

}

//�Փ˔���
void Switch::hitColl(Player& player)
{
	//�v���C���[�̈ʒu�������ɂ����J�v�Z���ƃX�C�b�`���f���̔���
	hitDim_.push_back(MV1CollCheck_Capsule(model_->getModelHandle(), model_->getColFrameIndex(), VAdd(player.getPos(), VGet(0.0f, 10.0f, 0.0f)), VAdd(player.getPos(), VGet(0.0f, 150.0f, 0.0f)), 20.0f));

	//���̂̈ʒu�������ɂ����J�v�Z���ƃX�C�b�`���f���̔���
	for (auto& person : player.getDeadPerson()) {
		VECTOR rot = deadPersonCapsulePosAdjustment(person);
		hitDim_.push_back(MV1CollCheck_Capsule(model_->getModelHandle(), model_->getColFrameIndex(), VAdd(person->getPos(), VGet(0.0f, 0.0f, 0.0f)), VAdd(person->getPos(), rot), 20.0f));
	}

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
	model_->setAnimation(1, false, false);
}

//�X�C�b�`�I�t�A�j���[�V����
void Switch::OffAnim()
{
	model_->setAnimation(2, false, false);
}
