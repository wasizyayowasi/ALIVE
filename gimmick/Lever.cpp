#include "Lever.h"
#include "../util/Model.h"

namespace {
	const char* const filename = "data/model/other/mv1/Lever.mv1";
}

//�R���X�g���N�^
Lever::Lever(LoadObjectInfo info, VECTOR stopPos)
{
	//���f���N���X�̃C���X�^���X��
	model_ = std::make_shared<Model>(filename);

	//�|�W�V�����̏�����
	pos_ = info.pos;

	//�G���x�[�^�[���~������|�W�V����
	elevatorStopPosition_ = stopPos;

	//���f���̏�����
	model_->SetPos(pos_);
	model_->SetScale(info.scale);
	model_->SetRot(info.rot);

}

//�f�X�g���N�^
Lever::~Lever()
{
}

//�X�V
void Lever::Update()
{
	//�X�V
	model_->Update();

	//�A�j���[�V�������~�A�j���[�V�����ɂ���
	if (model_->IsAnimEnd()) {
		OffAnimation();
	}
}

//�`��
void Lever::Draw()
{
	//�`��
	model_->Draw();
}

//�Փ˔���
bool Lever::CollCheck(VECTOR playerPos)
{
	//�v���C���[�ƃ��o�[���f���̏Փ˔���
	auto result = MV1CollCheck_Sphere(model_->GetModelHandle(), model_->GetColFrameIndex(), playerPos, 50.0f);

	if (result.HitNum > 0) {
		MV1CollResultPolyDimTerminate(result);
		return true;
	}

	return false;
}

//���o�[�̋N��
void Lever::OnAnimation()
{
	model_->ChangeAnimation(static_cast<int>(AnimType::on), false, false, 10);
	isOn_ = true;
}

//���o�[�̒�~
void Lever::OffAnimation()
{
	model_->ChangeAnimation(static_cast<int>(AnimType::off), false, false, 10);
	isOn_ = false;
}