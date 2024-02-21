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
	constexpr float max_rot_Z = -630.0f;

	//�Փ˔���p�J�v�Z���̔��a
	constexpr float radius_capsule = 60.0f;

	//�ŏ��Փː�
	constexpr int min_hit_num = 1;
}

//�R���X�g���N�^
ManualCrank::ManualCrank(const LoadObjectInfo objInfo)
{
	//���f���N���X�̏�����
	model_ = std::make_shared<Model>(ModelManager::GetInstance().GetModelHandle(objData_[static_cast<int>(ObjectType::Crank)].name), Material::Iron);
	model_->SetUseCollision(true,false);
	model_->SetScale(objInfo.scale);
	model_->SetPos(objInfo.pos);

	//�|�W�V�����̏�����
	pos_ = objInfo.pos;

	//�N�����N���񂷗����ʒu���擾
	int standingNum = StrUtil::GetNumberFromString(objInfo.name, ".");
	std::string standingName = StrUtil::GetConcatenateNumAndStrings("StandingPosition", ".", standingNum);

	//���ʒu�̏�����
	standingPos_ = ExternalFile::GetInstance().GetSpecifiedGimmickInfo(pos_, standingName.c_str()).pos;

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
	
	MV1_COLL_RESULT_POLY_DIM result;

	VECTOR playerPos = player.GetStatus().pos;

	MV1RefreshCollInfo(model_->GetModelHandle(), model_->GetColFrameIndex());
	result = MV1CollCheck_Sphere(model_->GetModelHandle(), model_->GetColFrameIndex(), playerPos, radius_capsule);

	if (result.HitNum < min_hit_num)
	{
		MV1CollResultPolyDimTerminate(result);
		return false;
	}

	MV1CollResultPolyDimTerminate(result);

	return true;
}

//Z���̍ő��]�����擾����
float ManualCrank::GetMaxRotZ() const
{
	return max_rot_Z;
}
