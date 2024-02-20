#include "Lever.h"

#include "../util/Model.h"
#include "../util/Util.h"
#include "../util/ExternalFile.h"
#include "../util/SoundManager.h"
#include "../util/ModelManager.h"

namespace 
{
	//�A�j���[�V�����̕ύX����
	constexpr int anim_change_time = 10;

	//�Փ˔���p���̔��a
	constexpr float radius_sphere = 100.0f;
}

//�R���X�g���N�^
Lever::Lever(const LoadObjectInfo info)
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();
	auto& model = ModelManager::GetInstance();

	//���f���N���X�̃C���X�^���X��
	model_ = std::make_shared<Model>(model.GetModelHandle(objData[static_cast<int>(ObjectType::Lever)].name), Material::Iron);

	//�|�W�V�����̏�����
	pos_ = info.pos;

	//���f���̏�����
	model_->SetPos(pos_);
	model_->SetScale(info.scale);
	model_->SetRot(info.rot);

	//���o�[���ǂ̃G���x�[�^�[�O���[�v�ɑ����Ă��邩�擾����
	int standingNum = StrUtil::GetNumberFromString(info.name, ".");
	std::string standingName = StrUtil::GetConcatenateNumAndStrings("LeverStandingPos", ".", standingNum);

	//�擾�����O���[�v�̉��Ԗڂ̃��o�[�����擾����
	int num = StrUtil::GetNumberFromString(info.name, "-");
	standingName = StrUtil::GetConcatenateNumAndStrings(standingName, "-", num);

	//���ʒu�̏�����
	standingPos_ = file.GetSpecifiedGimmickInfo(pos_, standingName.c_str()).pos;
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
bool Lever::CollCheck(const VECTOR playerPos) const
{
	//�v���C���[�ƃ��o�[���f���̏Փ˔���
	auto result = MV1CollCheck_Sphere(model_->GetModelHandle(), model_->GetColFrameIndex(), playerPos, radius_sphere);

	if (result.HitNum > 0)
	{
		MV1CollResultPolyDimTerminate(result);
		return true;
	}

	return false;
}

//���o�[�̋N��
void Lever::OnAnimation()
{
	model_->ChangeAnimation(static_cast<int>(AnimType::on), false, false, anim_change_time);
	isOn_ = true;
}

//���o�[�̒�~
void Lever::OffAnimation()
{
	model_->ChangeAnimation(static_cast<int>(AnimType::off), false, false, anim_change_time);
	isOn_ = false;
}