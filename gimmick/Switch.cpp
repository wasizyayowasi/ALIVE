#include "Switch.h"
#include "../util/Model.h"
#include "../OBJECT/Player.h"

namespace {
	//���f���̃t�@�C���p�X
	const char* const switch_filename = "data/model/other/mv1/switch.mv1";
}

//�R���X�g���N�^
Switch::Switch(LoadObjectInfo objInfo)
{
	//���f���N���X�̏�����
	model_ = std::make_shared<Model>(switch_filename);
	model_->SetScale(objInfo.scale);
	model_->SetPos(objInfo.pos);
	model_->SetRot(objInfo.rot);
	model_->SetCollFrame();

	//�|�W�V�����̏�����
	pos_ = objInfo.pos;

}

//�f�X�g���N�^
Switch::~Switch()
{
}

//�X�V
void Switch::Update(Player& player)
{
	//�A�j���[�V�����̍X�V
	model_->Update();

	//�Փ˔���
	HitCollPlayer(player);

	isTransit_ = player.GetStatus().situation.isInTransit;

}

//�`��
void Switch::Draw()
{
	model_->Draw();
//	DrawCapsule3D(pos_, VAdd(pos_, VGet(0.0f, 50.0f, 0.0f)), 50, 32, 0x00ff00, 0x00ff00, true);
//	DrawFormatString(0, 48, 0x448844, "%.2f,%.2f,%.2f", pos_.x, pos_.y, pos_.z);
}

void Switch::DeleteHitResult()
{
	//�Փˌ��ʂ̍폜
	for (auto& result : hitDim_) {
		MV1CollResultPolyDimTerminate(result);
	}

	//�Փˌ��ʂ�ێ�����z��̍폜
	hitDim_.clear();
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

	int temp = deadPerson->GetModelPointer()->GetColFrameIndex();

	//�����^�ђ���������ȍ~�̏������s��Ȃ�
	if (deadPerson->IsTransit()) {
		return;
	}

	//�v���C���[�̈ʒu�������ɂ����J�v�Z���ƃX�C�b�`���f���̔���
	hitDim_.push_back(MV1CollCheck_Capsule(deadPerson->GetModelPointer()->GetModelHandle(), deadPerson->GetModelPointer()->GetColFrameIndex(), pos_, VAdd(pos_, VGet(0.0f, 50.0f, 0.0f)), 50.0f));
}

//�Փ˔��茋�ʂ̏�����
bool Switch::CollResult()
{
	int hitNum = 0;
	//�������Ă��鐔�𐔂���
	for (auto& result : hitDim_) {
		if (result.HitNum > 0) {
			hitNum++;
		}
	}

	DeleteHitResult();

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

//�Փ˔�����s�����f���̒ǉ�
std::shared_ptr<Model> Switch::GetModelPointer()
{
	return model_;
}

//�X�C�b�`�I���A�j���[�V����
void Switch::OnAnim()
{
	model_->ChangeAnimation(0, false, false, 10);
	MV1SetMaterialDifColor(model_->GetModelHandle(), 1, GetColorF(0.0f, 0.0f, 1.0f, 1.0f));
}

//�X�C�b�`�I�t�A�j���[�V����
void Switch::OffAnim()
{
	model_->ChangeAnimation(1, false, false, 10);
	MV1SetMaterialDifColor(model_->GetModelHandle(), 1, GetColorF(1.0f, 0.0f, 0.0f, 1.0f));
}
