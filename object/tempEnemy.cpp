#include "tempEnemy.h"
#include "../util/Model.h"
#include "../object/Player.h"

namespace {
	//�ǐՂ��~�߂�͈�
	constexpr float range_to_stop_tracking = 60.0f;

	//���f���̏�����]�x�N�g��
	const VECTOR init_rot = { 0.0f,0.0f,-1.0f };

	//�G�̎���p
	constexpr float viewing_angle = 30.0f;
	constexpr float visible_range = 500.0f;
}

tempEnemy::tempEnemy(const char* const filename, LoadObjectInfo objInfo):EnemyBase(filename,objInfo)
{
	model_->SetScale(objInfo.scale);
	model_->SetPos(objInfo.pos);
	model_->SetRot(objInfo.rot);
	scale_ = objInfo.scale;
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
	//�v���C���[�̍��W
	VECTOR playerPos = player.GetStatus().pos;
	
	//���G
	SearchForPlayer(playerPos);
	
	//���f���̍X�V
	model_->Update();

	//�v���C���[�ƓG�̍��W�������āA
	if (distance_ < range_to_stop_tracking) {
		return;
	}
	//�G�̎���p�����O���ɂ���܂���
	//�G����v���C���[�̋������w��͈͂��傫��������return
	if (innerProduct > viewing_angle || distance_ > visible_range) {
		return;
	}

	//�v���C���[��ǐՂ���
	TrackingUpdate(playerPos);
}

void tempEnemy::Draw()
{
	model_->Draw();
}

//�ǐ�
void tempEnemy::TrackingUpdate(VECTOR playerPos)
{
	//�v���C���[�Ǝ����̍����Z�o���A���K�����A�X�s�[�h���|����
	VECTOR moveVec = VScale(VNorm(VSub(playerPos, pos_)), 3.0f);

	//��]�s��̎擾
	MATRIX rotMtx = MGetRotVec2(VGet(0, 0, -1), VSub(playerPos, pos_));

	//�g�k�s��̎擾
	MATRIX scaleMtx = MGetScale(scale_);

	//���ʃx�N�g�����擾����
	frontVector_ = VTransformSR(VGet(0, 0, -1), rotMtx);

	//��]�s��Ɗg�k�s��̊|���Z
	MATRIX mtx = MMult(rotMtx, scaleMtx);

	//�|�W�V�����̈ړ�
	pos_ = VAdd(pos_, moveVec);

	//��]�s��Ɗg�k�s����|�����s��ɕ��s�ړ��s���������
	mtx = MMult(mtx, MGetTranslate(pos_));

	//�s������f���ɃZ�b�g����
	MV1SetMatrix(model_->GetModelHandle(), mtx);
}

//���G
void tempEnemy::SearchForPlayer(VECTOR playerPos)
{
	//�G����v���C���[�̒�������
	distance_ = VSize(VSub(playerPos, pos_));

	//���ς��擾����(�Ԃ�l�̓R�T�C��)
	innerProduct = VDot(frontVector_, VNorm(VSub(playerPos, pos_)));

	//��L�̌��ʂ���x���@�ɕς���
	float aiu = acos(innerProduct);
	innerProduct = aiu / DX_PI_F * 180.0f;

}
