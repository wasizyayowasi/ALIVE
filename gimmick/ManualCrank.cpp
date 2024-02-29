#include "ManualCrank.h"

#include "../util/Util.h"
#include "../util/Model.h"
#include "../object/Player.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/ModelManager.h"

#include <algorithm>
#include <math.h>

namespace
{
	//�ő��]�l
	constexpr float max_rot_Z = -359.0f;

	//�Փ˔���p�J�v�Z���̔��a
	constexpr float radius_capsule = 60.0f;

	//�ŏ��Փː�
	constexpr int min_hit_num = 1;
}

//�R���X�g���N�^
ManualCrank::ManualCrank(const LoadObjectInfo& objInfo)
{
	//���f���N���X�̏�����
	model_ = std::make_shared<Model>(ModelManager::GetInstance().GetModelHandle(objData_[static_cast<int>(ObjectType::Crank)].name), Material::Iron);
	model_->SetUseCollision(true,false);
	model_->SetScale(objInfo.scale);
	model_->SetPos(objInfo.pos);

	//�N�����N���񂷗����ʒu���擾
	int standingNum = StrUtil::GetNumberFromString(objInfo.name, ".");
	LoadObjectInfo standingData = GetLoadObjectInfo("StandingPosition", standingNum);

	//���ʒu�̏�����
	standingPos_ = standingData.pos;

	//�|�W�V�����̏�����
	pos_ = objInfo.pos;
}

//�f�X�g���N�^
ManualCrank::~ManualCrank()
{
}

//�`��
void ManualCrank::Draw()
{
	//�`��
	model_->Draw();
}

//�v���C���[�Ƃ̏Փ˔���
bool ManualCrank::HitCollPlayer(Player& player) const
{
	//�Փˌ��ʗp�ϐ�
	MV1_COLL_RESULT_POLY_DIM result = {};

	//�v���C���[�̃|�W�V����
	VECTOR playerPos = player.GetStatus().pos;

	//�Փ˗p���̃��t���b�V��
	MV1RefreshCollInfo(model_->GetModelHandle(), model_->GetColFrameIndex());

	//�Փ˔���
	result = MV1CollCheck_Sphere(model_->GetModelHandle(), model_->GetColFrameIndex(), playerPos, radius_capsule);

	//�Փː���0�ȏゾ������true��Ԃ�
	if (result.HitNum > min_hit_num)
	{
		//�Փ˔���̌�n��
		MV1CollResultPolyDimTerminate(result);
		return true;
	}

	//�Փ˔���̌�n��
	MV1CollResultPolyDimTerminate(result);

	return false;
}

//Z���̍ő��]�����擾����
float ManualCrank::GetMaxRotZ() const
{
	return max_rot_Z;
}

//�w��̃I�u�W�F�N�g�̔z�u�f�[�^���擾����
LoadObjectInfo ManualCrank::GetLoadObjectInfo(const std::string& name, int num)
{
	//�Z�k��
	auto& file = ExternalFile::GetInstance();

	//���O�ɔԍ��𑫂�����������擾����
	std::string str = StrUtil::GetConcatenateNumAndStrings(name, ".", num);

	//��L�Ŏ擾����������̃I�u�W�F�N�g�̔z�u�f�[�^���擾����	
	LoadObjectInfo objInfo = file.GetSpecifiedGimmickInfo(str);

	//�z�u�f�[�^��Ԃ�
	return objInfo;
}