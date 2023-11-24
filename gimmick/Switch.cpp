#include "Switch.h"
#include "../util/Model.h"
#include "../OBJECT/Player.h"

namespace {

	const char* const switch_filename = "data/model/switch.mv1";

	const VECTOR scale = { 0.5f,0.5f, 0.5f };
}

Switch::Switch(LoadObjectInfo objInfo)
{
	model_ = std::make_shared<Model>(switch_filename);
	model_->SetScale(objInfo.scale);
	model_->SetPos(objInfo.pos);
	model_->SetRot(objInfo.rot);
	model_->SetCollFrame();

	pos_ = model_->GetPos();

}

Switch::~Switch()
{
}

//�X�V
void Switch::Update(Player& player)
{

	//�Փˌ��ʂ̍폜
	for (auto& result : hitDim_) {
		MV1CollResultPolyDimTerminate(result);
	}

	//�Փˌ��ʂ�ێ�����z��̍폜
	hitDim_.clear();

	//�A�j���[�V�����̍X�V
	model_->Update();

	//�Փ˔���
	HitCollPlayer(player);

	isTransit_ = player.GetStatus().situation.isInTransit;

}

void Switch::Draw()
{
	model_->Draw();
//	DrawFormatString(0, 48, 0x448844, "%.2f,%.2f,%.2f", pos_.x, pos_.y, pos_.z);
}

//�Փ˔���
void Switch::HitCollPlayer(Player& player)
{

	VECTOR playerPos = player.GetStatus().pos;

	MV1RefreshCollInfo(model_->GetModelHandle(), model_->GetColFrameIndex());

	int model = model_->GetModelHandle();

	//�v���C���[�̈ʒu�������ɂ����J�v�Z���ƃX�C�b�`���f���̔���
	hitDim_.push_back(MV1CollCheck_Capsule(model_->GetModelHandle(), model_->GetColFrameIndex(), playerPos, VAdd(playerPos, VGet(0.0f, player.GetStatus().height, 0.0f)), 20.0f));
}

void Switch::HitColl(std::shared_ptr<ObjectBase> deadPerson)
{
	MV1RefreshCollInfo(deadPerson->GetModelPointer()->GetModelHandle(), deadPerson->GetModelPointer()->GetColFrameIndex());

	//�����^�ђ���������ȍ~�̏������s��Ȃ�
	if (deadPerson->IsTransit()) {
		return;
	}

	//�v���C���[�̈ʒu�������ɂ����J�v�Z���ƃX�C�b�`���f���̔���
	hitDim_.push_back(MV1CollCheck_Capsule(deadPerson->GetModelPointer()->GetModelHandle(), deadPerson->GetModelPointer()->GetColFrameIndex(), pos_, VAdd(pos_, VGet(0.0f, 50.0f, 0.0f)), 50.0f));
}

bool Switch::CollResult()
{
	int hitNum = 0;
	//�������Ă��鐔�𐔂���
	for (auto& result : hitDim_) {
		if (result.HitNum > 0) {
			hitNum++;
		}
	}

	//�������Ă��Ȃ�������
	//�A�j���[�V������ύX���I��
	if (hitNum < 1) {
		OffAnim();
		return false;
	}

	//�A�j���[�V�����̕ύX
	OnAnim();

	return true;
}

std::shared_ptr<Model> Switch::GetModelPointer()
{
	return model_;
}

//�X�C�b�`�I���A�j���[�V����
void Switch::OnAnim()
{
	MV1SetMaterialDifColor(model_->GetModelHandle(), 1, GetColorF(0.0f, 0.0f, 1.0f, 1.0f));
}

//�X�C�b�`�I�t�A�j���[�V����
void Switch::OffAnim()
{
	MV1SetMaterialDifColor(model_->GetModelHandle(), 1, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
}
