#include "Corpse.h"

#include "Player.h"

#include "../util/Model.h"

namespace {
	//���f���t���[����
	const char* const coll_frame_death = "CollDown";
}

//�R���X�g���N�^
Corpse::Corpse(int handle,Material materialType, LoadObjectInfo objInfo, int animNo) : CharacterBase(handle, materialType,objInfo)
{
	//�Փ˔�����s��
	isCollCheck_ = true;

	//�Փ˔�����s�����f���t���[���̍X�V���K�v���̐ݒ���s��
	isUpdateColl_ = true;

	//�}�e���A���̐F��ύX����
	MV1SetMaterialDifColor(model_->GetModelHandle(), 8, GetColorF(1.0f,0.0f,0.0f,1.0f));

	//�A�j���[�V�����̐ݒ�
	model_->SetAnimation(animNo, false, true);

	//�A�j���[�V�����̎w��̍Đ����Ԃ�ݒ肷��
	model_->SetAnimEndFrame(animNo);

	//�Փ˔�����s�����Ƃ�ݒ肷��
	model_->SetUseCollision(isCollCheck_, isUpdateColl_,coll_frame_death);
}

//�f�X�g���N�^
Corpse::~Corpse()
{
}

//�X�V
void Corpse::Update(Player& player)
{
	pos_ = model_->GetPos();
}

//�`��
void Corpse::Draw()
{
	model_->Draw();
}