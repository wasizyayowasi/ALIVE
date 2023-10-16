#include "tempEnemy.h"
#include "../util/Model.h"
#include "../object/Player.h"

namespace {
	//�ǐՂ��~�߂�͈�
	constexpr float range_to_stop_tracking = 30.0f;
}

tempEnemy::tempEnemy(const char* const filename, LoadObjectInfo objInfo):EnemyBase(filename,objInfo)
{
	model_->SetScale(objInfo.scale);
	model_->SetPos(objInfo.pos);
	model_->SetRot(objInfo.rot);
}

tempEnemy::tempEnemy(int handle, LoadObjectInfo objInfo) : EnemyBase(handle,objInfo)
{
	model_->SetScale(objInfo.scale);
	model_->SetPos(objInfo.pos);
	model_->SetRot(objInfo.rot);
	scale_ = objInfo.scale;
}

tempEnemy::~tempEnemy()
{
}

void tempEnemy::Update(Player& player)
{
	model_->Update();

	//�v���C���[�ƓG�̍��W�������āA
	VECTOR between = VSub(player.GetStatus().pos, pos_);
	if (VSize(between) < range_to_stop_tracking) {
		return;
	}

	//�v���C���[��ǐՂ���
	TrackingUpdate(player);
}

void tempEnemy::Draw()
{
	model_->Draw();
}

void tempEnemy::TrackingUpdate(Player& player)
{
	//�v���C���[�Ǝ����̍����Z�o���A���K�����A�X�s�[�h���|����
	VECTOR moveVec = VScale(VNorm(VSub(player.GetStatus().pos, pos_)), 3.0f);

	//��]�s��̎擾
	MATRIX rotMtx = MGetRotVec2(VGet(0, 0, -1), VSub(player.GetStatus().pos, pos_));

	//�g�k�s��̎擾
	MATRIX scaleMtx = MGetScale(scale_);

	//��]�s��Ɗg�k�s��̊|���Z
	MATRIX mtx = MMult(rotMtx, scaleMtx);

	//�|�W�V�����̈ړ�
	pos_ = VAdd(pos_, moveVec);

	//��]�s��Ɗg�k�s����|�����s��ɕ��s�ړ��s���������
	mtx = MMult(mtx, MGetTranslate(pos_));

	//�s������f���ɃZ�b�g����
	MV1SetMatrix(model_->GetModelHandle(), mtx);
}
