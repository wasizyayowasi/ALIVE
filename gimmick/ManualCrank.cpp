#include "ManualCrank.h"
#include "../util/Model.h"
#include "../util/InputState.h"
#include "../util/ExternalFile.h"
#include "../util/Util.h"
#include "../object/Player.h"
#include <algorithm>
#include <math.h>


namespace {
	//�N�����N���f���̃t�@�C���p�X
	const char* const crank_filename = "data/model/other/mv1/Crank.mv1";

	//�ő��]�l
	constexpr float max_rot_Z = -630.0f;
}

//�R���X�g���N�^
ManualCrank::ManualCrank(LoadObjectInfo objInfo)
{
	//���f���N���X�̏�����
	model_ = std::make_shared<Model>(crank_filename);
	model_->SetCollFrame();
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
bool ManualCrank::HitCollPlayer(Player& player)
{
	
	MV1_COLL_RESULT_POLY_DIM result;

	VECTOR playerPos = player.GetStatus().pos;

	MV1RefreshCollInfo(model_->GetModelHandle(), model_->GetColFrameIndex());
	result = MV1CollCheck_Capsule(model_->GetModelHandle(), model_->GetColFrameIndex(), playerPos, VAdd(playerPos, VGet(0, player.GetStatus().height, 0)),80.0f);

	if (result.HitNum < 1) {
		MV1CollResultPolyDimTerminate(result);
		return false;
	}

	MV1CollResultPolyDimTerminate(result);

	return true;
}

//Z���̍ő��]�����擾����
float ManualCrank::GetMaxRotZ()
{
	return max_rot_Z;
}
